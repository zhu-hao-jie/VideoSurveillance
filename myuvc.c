#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/module.h>
#include <linux/usb.h>
#include <linux/videodev2.h>
#include <linux/vmalloc.h>
#include <linux/wait.h>

#include <linux/mm.h>
#include <asm/atomic.h>
#include <asm/unaligned.h>

#include <media/v4l2-common.h>
#include <media/v4l2-ioctl.h>
#include <media/videobuf-core.h>

#include "uvcvideo.h"

/* �ο� drivers/media/video/uvc */

#define MYUVC_URBS 5

/* Values for bmHeaderInfo (Video and Still Image Payload Headers, 2.4.3.3) */
#define UVC_STREAM_EOH	(1 << 7)
#define UVC_STREAM_ERR	(1 << 6)
#define UVC_STREAM_STI	(1 << 5)
#define UVC_STREAM_RES	(1 << 4)
#define UVC_STREAM_SCR	(1 << 3)
#define UVC_STREAM_PTS	(1 << 2)
#define UVC_STREAM_EOF	(1 << 1)
#define UVC_STREAM_FID	(1 << 0)


struct myuvc_streaming_control {
	__u16 bmHint;
	__u8  bFormatIndex;
	__u8  bFrameIndex;
	__u32 dwFrameInterval;
	__u16 wKeyFrameRate;
	__u16 wPFrameRate;
	__u16 wCompQuality;
	__u16 wCompWindowSize;
	__u16 wDelay;
	__u32 dwMaxVideoFrameSize;
	__u32 dwMaxPayloadTransferSize;
	__u32 dwClockFrequency;
	__u8  bmFramingInfo;
	__u8  bPreferedVersion;
	__u8  bMinVersion;
	__u8  bMaxVersion;
};



struct frame_desc {
    int width;
    int height;
};

/* �ο�uvc_video_queue����һЩ�ṹ�� */
struct myuvc_buffer {    
    struct v4l2_buffer buf;
    int state;
    int vma_use_count; /* ��ʾ�Ƿ��Ѿ���mmap */
    wait_queue_head_t wait;  /* APPҪ��ĳ��������,���������,�ڴ����� */
	struct list_head stream;
	struct list_head irq;    
};

struct myuvc_queue {
    void *mem;
    int count;
    int buf_size;    
    struct myuvc_buffer buffer[32];

	struct urb *urb[32];
	char *urb_buffer[32];
	dma_addr_t urb_dma[32];
	unsigned int urb_size;

	struct list_head mainqueue;   /* ��APP������ */
	struct list_head irqqueue;    /* ���ײ����������� */
};

static struct myuvc_queue myuvc_queue;

static struct video_device *myuvc_vdev;
static struct usb_device *myuvc_udev;
static int myuvc_bEndpointAddress = 0x81;
static int myuvc_streaming_intf;
static int myuvc_control_intf;
static int myuvc_streaming_bAlternateSetting = 8;
static struct v4l2_format myuvc_format;

static struct frame_desc frames[] = {{640, 480}, {352, 288}, {320, 240}, {176, 144}, {160, 120}};
static int frame_idx = 1;
static int bBitsPerPixel = 16; /* lsusb -v -d 0x1e4e:  "bBitsPerPixel" */
static int uvc_version = 0x0100; /* lsusb -v -d 0x1e4e: bcdUVC */

static int wMaxPacketSize = 1024;
static int ProcessingUnitID = 3;

static struct myuvc_streaming_control myuvc_params;


/* A2 �ο� uvc_v4l2_do_ioctl */
static int myuvc_vidioc_querycap(struct file *file, void  *priv,
					struct v4l2_capability *cap)
{    
    memset(cap, 0, sizeof *cap);
    strcpy(cap->driver, "myuvc");
    strcpy(cap->card, "myuvc");
    cap->version = 1;
    
    cap->capabilities = V4L2_CAP_VIDEO_CAPTURE | V4L2_CAP_STREAMING;
 
	return 0;
}

/* A3 �о�֧�����ָ�ʽ
 * �ο�: uvc_fmts ����
 */
static int myuvc_vidioc_enum_fmt_vid_cap(struct file *file, void  *priv,
					struct v4l2_fmtdesc *f)
{
    /* �˹��鿴��������֪�����õ�����ͷֻ֧��1�ָ�ʽ */
	if (f->index >= 1)
		return -EINVAL;

    /* ֧��ʲô��ʽ��?
     * �鿴VideoStreaming Interface��������,
     * �õ�GUIDΪ"59 55 59 32 00 00 10 00 80 00 00 aa 00 38 9b 71"
     */
	strcpy(f->description, "4:2:2, packed, YUYV");
	f->pixelformat = V4L2_PIX_FMT_YUYV;    
    
	return 0;
}

/* A4 ���ص�ǰ��ʹ�õĸ�ʽ */
static int myuvc_vidioc_g_fmt_vid_cap(struct file *file, void *priv,
					struct v4l2_format *f)
{
    memcpy(f, &myuvc_format, sizeof(myuvc_format));
	return (0);
}

/* A5 �������������Ƿ�֧��ĳ�ָ�ʽ, ǿ�����øø�ʽ 
 * �ο�: uvc_v4l2_try_format
 *       myvivi_vidioc_try_fmt_vid_cap
 */
static int myuvc_vidioc_try_fmt_vid_cap(struct file *file, void *priv,
			struct v4l2_format *f)
{
    if (f->type != V4L2_BUF_TYPE_VIDEO_CAPTURE)
    {
        return -EINVAL;
    }

    if (f->fmt.pix.pixelformat != V4L2_PIX_FMT_YUYV)
        return -EINVAL;
    
    /* ����format��width, height, 
     * ����bytesperline, sizeimage
     */

    /* �˹��鿴������, ȷ��֧���ļ��ֱַ��� */
    f->fmt.pix.width  = frames[frame_idx].width;
    f->fmt.pix.height = frames[frame_idx].height;
    
	f->fmt.pix.bytesperline =
		(f->fmt.pix.width * bBitsPerPixel) >> 3;
	f->fmt.pix.sizeimage =
		f->fmt.pix.height * f->fmt.pix.bytesperline;
    
    return 0;
}

/* A6 �ο� myvivi_vidioc_s_fmt_vid_cap */
static int myuvc_vidioc_s_fmt_vid_cap(struct file *file, void *priv,
					struct v4l2_format *f)
{
	int ret = myuvc_vidioc_try_fmt_vid_cap(file, NULL, f);
	if (ret < 0)
		return ret;

    memcpy(&myuvc_format, f, sizeof(myuvc_format));
    
    return 0;
}

static int myuvc_free_buffers(void)
{
    if (myuvc_queue.mem)
    {
        vfree(myuvc_queue.mem);
        memset(&myuvc_queue, 0, sizeof(myuvc_queue));
        myuvc_queue.mem = NULL;
    }
    return 0;
}

/* A7 APP���ø�ioctl����������������ɸ�����, APP������Щ�����ж�����Ƶ���� 
 * �ο�: uvc_alloc_buffers
 */
static int myuvc_vidioc_reqbufs(struct file *file, void *priv,
			  struct v4l2_requestbuffers *p)
{
    int nbuffers = p->count;
    int bufsize  = PAGE_ALIGN(myuvc_format.fmt.pix.sizeimage);
    unsigned int i;
    void *mem = NULL;
    int ret;

    if ((ret = myuvc_free_buffers()) < 0)
        goto done;

    /* Bail out if no buffers should be allocated. */
    if (nbuffers == 0)
        goto done;

    /* Decrement the number of buffers until allocation succeeds. */
    for (; nbuffers > 0; --nbuffers) {
        mem = vmalloc_32(nbuffers * bufsize);
        if (mem != NULL)
            break;
    }

    if (mem == NULL) {
        ret = -ENOMEM;
        goto done;
    }

    /* ��Щ������һ������Ϊһ������������� */
    memset(&myuvc_queue, 0, sizeof(myuvc_queue));

	INIT_LIST_HEAD(&myuvc_queue.mainqueue);
	INIT_LIST_HEAD(&myuvc_queue.irqqueue);

    for (i = 0; i < nbuffers; ++i) {
        myuvc_queue.buffer[i].buf.index = i;
        myuvc_queue.buffer[i].buf.m.offset = i * bufsize;
        myuvc_queue.buffer[i].buf.length = myuvc_format.fmt.pix.sizeimage;
        myuvc_queue.buffer[i].buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        myuvc_queue.buffer[i].buf.sequence = 0;
        myuvc_queue.buffer[i].buf.field = V4L2_FIELD_NONE;
        myuvc_queue.buffer[i].buf.memory = V4L2_MEMORY_MMAP;
        myuvc_queue.buffer[i].buf.flags = 0;
        myuvc_queue.buffer[i].state     = VIDEOBUF_IDLE;
        init_waitqueue_head(&myuvc_queue.buffer[i].wait);
    }

    myuvc_queue.mem = mem;
    myuvc_queue.count = nbuffers;
    myuvc_queue.buf_size = bufsize;
    ret = nbuffers;

done:
    return ret;
}

/* A8 ��ѯ����״̬, �����ַ��Ϣ(APP������mmap����ӳ��) 
 * �ο� uvc_query_buffer
 */
static int myuvc_vidioc_querybuf(struct file *file, void *priv, struct v4l2_buffer *v4l2_buf)
{
    int ret = 0;
    
	if (v4l2_buf->index >= myuvc_queue.count) {
		ret = -EINVAL;
		goto done;
	}

    memcpy(v4l2_buf, &myuvc_queue.buffer[v4l2_buf->index].buf, sizeof(*v4l2_buf));

    /* ����flags */
	if (myuvc_queue.buffer[v4l2_buf->index].vma_use_count)
		v4l2_buf->flags |= V4L2_BUF_FLAG_MAPPED;


	switch (myuvc_queue.buffer[v4l2_buf->index].state) {
    	case VIDEOBUF_ERROR:
    	case VIDEOBUF_DONE:
    		v4l2_buf->flags |= V4L2_BUF_FLAG_DONE;
    		break;
    	case VIDEOBUF_QUEUED:
    	case VIDEOBUF_ACTIVE:
    		v4l2_buf->flags |= V4L2_BUF_FLAG_QUEUED;
    		break;
    	case VIDEOBUF_IDLE:
    	default:
    		break;
	}

done:    
	return ret;
}

/* A10 �ѻ������������, �ײ��Ӳ������������������ݷ���������еĻ��� 
 * �ο�: uvc_queue_buffer
 */
static int myuvc_vidioc_qbuf(struct file *file, void *priv, struct v4l2_buffer *v4l2_buf)
{
    struct myuvc_buffer *buf;

    /* 0. APP�����v4l2_buf����������, Ҫ���ж� */

	if (v4l2_buf->type != V4L2_BUF_TYPE_VIDEO_CAPTURE ||
	    v4l2_buf->memory != V4L2_MEMORY_MMAP) {
		return -EINVAL;
	}

	if (v4l2_buf->index >= myuvc_queue.count) {
		return -EINVAL;
	}

    buf = &myuvc_queue.buffer[v4l2_buf->index];

	if (buf->state != VIDEOBUF_IDLE) {
		return -EINVAL;
	}


    /* 1. �޸�״̬ */
	buf->state = VIDEOBUF_QUEUED;
	buf->buf.bytesused = 0;

    /* 2. ����2������ */
    /* ����1: ��APPʹ�� 
     * ��������û������ʱ,����mainqueue����
     * ��������������ʱ, APP��mainqueue������ȡ��
     */
	list_add_tail(&buf->stream, &myuvc_queue.mainqueue);

    /* ����2: ���������ݵĺ���ʹ��
     * ���ɼ�������ʱ,��irqqueue������ȡ����1��������,��������
     */
	list_add_tail(&buf->irq, &myuvc_queue.irqqueue);
    
	return 0;
}

static void myuvc_print_streaming_params(struct myuvc_streaming_control *ctrl)
{
    printk("video params:\n");
    printk("bmHint                   = %d\n", ctrl->bmHint);
    printk("bFormatIndex             = %d\n", ctrl->bFormatIndex);
    printk("bFrameIndex              = %d\n", ctrl->bFrameIndex);
    printk("dwFrameInterval          = %d\n", ctrl->dwFrameInterval);
    printk("wKeyFrameRate            = %d\n", ctrl->wKeyFrameRate);
    printk("wPFrameRate              = %d\n", ctrl->wPFrameRate);
    printk("wCompQuality             = %d\n", ctrl->wCompQuality);
    printk("wCompWindowSize          = %d\n", ctrl->wCompWindowSize);
    printk("wDelay                   = %d\n", ctrl->wDelay);
    printk("dwMaxVideoFrameSize      = %d\n", ctrl->dwMaxVideoFrameSize);
    printk("dwMaxPayloadTransferSize = %d\n", ctrl->dwMaxPayloadTransferSize);
    printk("dwClockFrequency         = %d\n", ctrl->dwClockFrequency);
    printk("bmFramingInfo            = %d\n", ctrl->bmFramingInfo);
    printk("bPreferedVersion         = %d\n", ctrl->bPreferedVersion);
    printk("bMinVersion              = %d\n", ctrl->bMinVersion);
    printk("bMinVersion              = %d\n", ctrl->bMinVersion);
}


/* �ο�: uvc_get_video_ctrl 
 (ret = uvc_get_video_ctrl(video, probe, 1, GET_CUR)) 
 static int uvc_get_video_ctrl(struct uvc_video_device *video,
     struct uvc_streaming_control *ctrl, int probe, __u8 query)
 */
static int myuvc_get_streaming_params(struct myuvc_streaming_control *ctrl)
{
	__u8 *data;
	__u16 size;
	int ret;
	__u8 type = USB_TYPE_CLASS | USB_RECIP_INTERFACE;
	unsigned int pipe;

	size = uvc_version >= 0x0110 ? 34 : 26;
	data = kmalloc(size, GFP_KERNEL);
	if (data == NULL)
		return -ENOMEM;
   
	pipe = (GET_CUR & 0x80) ? usb_rcvctrlpipe(myuvc_udev, 0)
			      : usb_sndctrlpipe(myuvc_udev, 0);
	type |= (GET_CUR & 0x80) ? USB_DIR_IN : USB_DIR_OUT;

	ret = usb_control_msg(myuvc_udev, pipe, GET_CUR, type, VS_PROBE_CONTROL << 8,
			0 << 8 | myuvc_streaming_intf, data, size, 5000);

    if (ret < 0)
        goto done;

	ctrl->bmHint = le16_to_cpup((__le16 *)&data[0]);
	ctrl->bFormatIndex = data[2];
	ctrl->bFrameIndex = data[3];
	ctrl->dwFrameInterval = le32_to_cpup((__le32 *)&data[4]);
	ctrl->wKeyFrameRate = le16_to_cpup((__le16 *)&data[8]);
	ctrl->wPFrameRate = le16_to_cpup((__le16 *)&data[10]);
	ctrl->wCompQuality = le16_to_cpup((__le16 *)&data[12]);
	ctrl->wCompWindowSize = le16_to_cpup((__le16 *)&data[14]);
	ctrl->wDelay = le16_to_cpup((__le16 *)&data[16]);
	ctrl->dwMaxVideoFrameSize = get_unaligned_le32(&data[18]);
	ctrl->dwMaxPayloadTransferSize = get_unaligned_le32(&data[22]);

	if (size == 34) {
		ctrl->dwClockFrequency = get_unaligned_le32(&data[26]);
		ctrl->bmFramingInfo = data[30];
		ctrl->bPreferedVersion = data[31];
		ctrl->bMinVersion = data[32];
		ctrl->bMaxVersion = data[33];
	} else {
		//ctrl->dwClockFrequency = video->dev->clock_frequency;
		ctrl->bmFramingInfo = 0;
		ctrl->bPreferedVersion = 0;
		ctrl->bMinVersion = 0;
		ctrl->bMaxVersion = 0;
	}

done:
    kfree(data);
    
    return (ret < 0) ? ret : 0;
}

/* �ο�: uvc_v4l2_try_format �Muvc_probe_video 
 *       uvc_set_video_ctrl(video, probe, 1)
 */
static int myuvc_try_streaming_params(struct myuvc_streaming_control *ctrl)
{
    __u8 *data;
    __u16 size;
    int ret;
	__u8 type = USB_TYPE_CLASS | USB_RECIP_INTERFACE;
	unsigned int pipe;
    
	memset(ctrl, 0, sizeof *ctrl);
    
	ctrl->bmHint = 1;	/* dwFrameInterval */
	ctrl->bFormatIndex = 1;
	ctrl->bFrameIndex  = frame_idx + 1;
	ctrl->dwFrameInterval = 333333;


    size = uvc_version >= 0x0110 ? 34 : 26;
    data = kzalloc(size, GFP_KERNEL);
    if (data == NULL)
        return -ENOMEM;

    *(__le16 *)&data[0] = cpu_to_le16(ctrl->bmHint);
    data[2] = ctrl->bFormatIndex;
    data[3] = ctrl->bFrameIndex;
    *(__le32 *)&data[4] = cpu_to_le32(ctrl->dwFrameInterval);
    *(__le16 *)&data[8] = cpu_to_le16(ctrl->wKeyFrameRate);
    *(__le16 *)&data[10] = cpu_to_le16(ctrl->wPFrameRate);
    *(__le16 *)&data[12] = cpu_to_le16(ctrl->wCompQuality);
    *(__le16 *)&data[14] = cpu_to_le16(ctrl->wCompWindowSize);
    *(__le16 *)&data[16] = cpu_to_le16(ctrl->wDelay);
    put_unaligned_le32(ctrl->dwMaxVideoFrameSize, &data[18]);
    put_unaligned_le32(ctrl->dwMaxPayloadTransferSize, &data[22]);

    if (size == 34) {
        put_unaligned_le32(ctrl->dwClockFrequency, &data[26]);
        data[30] = ctrl->bmFramingInfo;
        data[31] = ctrl->bPreferedVersion;
        data[32] = ctrl->bMinVersion;
        data[33] = ctrl->bMaxVersion;
    }

    pipe = (SET_CUR & 0x80) ? usb_rcvctrlpipe(myuvc_udev, 0)
                  : usb_sndctrlpipe(myuvc_udev, 0);
    type |= (SET_CUR & 0x80) ? USB_DIR_IN : USB_DIR_OUT;

    ret = usb_control_msg(myuvc_udev, pipe, SET_CUR, type, VS_PROBE_CONTROL << 8,
            0 << 8 | myuvc_streaming_intf, data, size, 5000);

    kfree(data);
    
    return (ret < 0) ? ret : 0;
    
}


/* �ο�: uvc_v4l2_try_format �Muvc_probe_video 
 *       uvc_set_video_ctrl(video, probe, 1)
 */
static int myuvc_set_streaming_params(struct myuvc_streaming_control *ctrl)
{
    __u8 *data;
    __u16 size;
    int ret;
	__u8 type = USB_TYPE_CLASS | USB_RECIP_INTERFACE;
	unsigned int pipe;
    
    size = uvc_version >= 0x0110 ? 34 : 26;
    data = kzalloc(size, GFP_KERNEL);
    if (data == NULL)
        return -ENOMEM;

    *(__le16 *)&data[0] = cpu_to_le16(ctrl->bmHint);
    data[2] = ctrl->bFormatIndex;
    data[3] = ctrl->bFrameIndex;
    *(__le32 *)&data[4] = cpu_to_le32(ctrl->dwFrameInterval);
    *(__le16 *)&data[8] = cpu_to_le16(ctrl->wKeyFrameRate);
    *(__le16 *)&data[10] = cpu_to_le16(ctrl->wPFrameRate);
    *(__le16 *)&data[12] = cpu_to_le16(ctrl->wCompQuality);
    *(__le16 *)&data[14] = cpu_to_le16(ctrl->wCompWindowSize);
    *(__le16 *)&data[16] = cpu_to_le16(ctrl->wDelay);
    put_unaligned_le32(ctrl->dwMaxVideoFrameSize, &data[18]);
    put_unaligned_le32(ctrl->dwMaxPayloadTransferSize, &data[22]);

    if (size == 34) {
        put_unaligned_le32(ctrl->dwClockFrequency, &data[26]);
        data[30] = ctrl->bmFramingInfo;
        data[31] = ctrl->bPreferedVersion;
        data[32] = ctrl->bMinVersion;
        data[33] = ctrl->bMaxVersion;
    }

    pipe = (SET_CUR & 0x80) ? usb_rcvctrlpipe(myuvc_udev, 0)
                  : usb_sndctrlpipe(myuvc_udev, 0);
    type |= (SET_CUR & 0x80) ? USB_DIR_IN : USB_DIR_OUT;

    ret = usb_control_msg(myuvc_udev, pipe, SET_CUR, type, VS_COMMIT_CONTROL << 8,
            0 << 8 | myuvc_streaming_intf, data, size, 5000);

    kfree(data);
    
    return (ret < 0) ? ret : 0;
    
}

static void myuvc_uninit_urbs(void)
{
    int i;
    for (i = 0; i < MYUVC_URBS; ++i) {
        if (myuvc_queue.urb_buffer[i])
        {
            usb_buffer_free(myuvc_udev, myuvc_queue.urb_size, myuvc_queue.urb_buffer[i], myuvc_queue.urb_dma[i]);
            myuvc_queue.urb_buffer[i] = NULL;
        }

        if (myuvc_queue.urb[i])
        {
            usb_free_urb(myuvc_queue.urb[i]);
            myuvc_queue.urb[i] = NULL;
        }
    }
}

/* �ο�: uvc_video_complete / uvc_video_decode_isoc */
static void myuvc_video_complete(struct urb *urb)
{
	u8 *src;
    u8 *dest;
	int ret, i;
    int len;
    int maxlen;
    int nbytes;
    struct myuvc_buffer *buf;
    
	switch (urb->status) {
	case 0:
		break;

	default:
		printk("Non-zero status (%d) in video "
			"completion handler.\n", urb->status);
		return;
	}

    /* ��irqqueue������ȡ����1�������� */
	if (!list_empty(&myuvc_queue.irqqueue))
	{
		buf = list_first_entry(&myuvc_queue.irqqueue, struct myuvc_buffer, irq);
    

    	for (i = 0; i < urb->number_of_packets; ++i) {
    		if (urb->iso_frame_desc[i].status < 0) {
    			printk("USB isochronous frame "
    				"lost (%d).\n", urb->iso_frame_desc[i].status);
    			continue;
    		}

            src  = urb->transfer_buffer + urb->iso_frame_desc[i].offset;

            dest = myuvc_queue.mem + buf->buf.m.offset + buf->buf.bytesused;

            len = urb->iso_frame_desc[i].actual_length;
            /* �ж������Ƿ���Ч */
            /* URB���ݺ���:
             * data[0] : ͷ������
             * data[1] : ����״̬
             */
            if (len < 2 || src[0] < 2 || src[0] > len)
                continue;
            
            /* Skip payloads marked with the error bit ("error frames"). */
            if (src[1] & UVC_STREAM_ERR) {
                printk("Dropping payload (error bit set).\n");
                continue;
            }

            /* ��ȥͷ��������ݳ��� */
            len -= src[0];

            /* ��������໹�ܴ�������� */
            maxlen = buf->buf.length - buf->buf.bytesused;
            nbytes = min(len, maxlen);

            /* �������� */
            memcpy(dest, src + src[0], nbytes);
            buf->buf.bytesused += nbytes;

            /* �ж�һ֡�����Ƿ��Ѿ�ȫ�����յ� */
            if (len > maxlen) {
                buf->state = VIDEOBUF_DONE;
            }
            
            /* Mark the buffer as done if the EOF marker is set. */
            if (src[1] & UVC_STREAM_EOF && buf->buf.bytesused != 0) {
                printk("Frame complete (EOF found).\n");
                if (len == 0)
                    printk("EOF in empty payload.\n");
                buf->state = VIDEOBUF_DONE;
            }

    	}

        /* ��������һ֡����, 
         * ��irqqueue��ɾ�����������
         * ���ѵȴ����ݵĽ��� 
         */
        if (buf->state == VIDEOBUF_DONE ||
            buf->state == VIDEOBUF_ERROR)
        {
            list_del(&buf->irq);
            wake_up(&buf->wait);
        }
	}

    /* �ٴ��ύURB */
	if ((ret = usb_submit_urb(urb, GFP_ATOMIC)) < 0) {
		printk("Failed to resubmit video URB (%d).\n", ret);
	}
}

/* �ο�: uvc_init_video_isoc */
static int myuvc_alloc_init_urbs(void)
{
	u16 psize;
	u32 size;
    int npackets;
    int i;
    int j;

    struct urb *urb;

	psize = wMaxPacketSize; /* ʵʱ����˵�һ���ܴ��������ֽ��� */
	size  = myuvc_params.dwMaxVideoFrameSize;  /* һ֡���ݵ���󳤶� */
    npackets = DIV_ROUND_UP(size, psize);
    if (npackets > 32)
        npackets = 32;

    size = myuvc_queue.urb_size = psize * npackets;
    
    for (i = 0; i < MYUVC_URBS; ++i) {
        /* 1. ����usb_buffers */
        
        myuvc_queue.urb_buffer[i] = usb_buffer_alloc(
            myuvc_udev, size,
            GFP_KERNEL | __GFP_NOWARN, &myuvc_queue.urb_dma[i]);

        /* 2. ����urb */
		myuvc_queue.urb[i] = usb_alloc_urb(npackets, GFP_KERNEL);

        if (!myuvc_queue.urb_buffer[i] || !myuvc_queue.urb[i])
        {
            myuvc_uninit_urbs();
            return -ENOMEM;
        }

    }


    /* 3. ����urb */
    for (i = 0; i < MYUVC_URBS; ++i) {
        urb = myuvc_queue.urb[i];
        
        urb->dev = myuvc_udev;
        urb->context = NULL;
        urb->pipe = usb_rcvisocpipe(myuvc_udev,myuvc_bEndpointAddress);
        urb->transfer_flags = URB_ISO_ASAP | URB_NO_TRANSFER_DMA_MAP;
        urb->interval = 1;
        urb->transfer_buffer = myuvc_queue.urb_buffer[i];
        urb->transfer_dma = myuvc_queue.urb_dma[i];
        urb->complete = myuvc_video_complete;
        urb->number_of_packets = npackets;
        urb->transfer_buffer_length = size;
        
        for (j = 0; j < npackets; ++j) {
            urb->iso_frame_desc[j].offset = j * psize;
            urb->iso_frame_desc[j].length = psize;
        }
    
    }
    
    return 0;
}

/* A11 �������� 
 * �ο�: uvc_video_enable(video, 1):
 *           uvc_commit_video
 *           uvc_init_video
 */
static int myuvc_vidioc_streamon(struct file *file, void *priv, enum v4l2_buf_type i)
{
    int ret;
    
    /* 1. ��USB����ͷ���ò���: ����ʹ���ĸ�format, ʹ�����format�µ��ĸ�frame(�ֱ���) 
     * �ο�: uvc_set_video_ctrl / uvc_get_video_ctrl
     * 1.1 ����һ���ṹ��uvc_streaming_control�������ݰ�: �����ֹ�����,Ҳ���Զ��������޸�
     * 1.2 ����usb_control_msg�������ݰ�
     */

    /* a. ���Բ��� */
    ret = myuvc_try_streaming_params(&myuvc_params);
    printk("myuvc_try_streaming_params ret = %d\n", ret);

    /* b. ȡ������ */
    ret = myuvc_get_streaming_params(&myuvc_params);
    printk("myuvc_get_streaming_params ret = %d\n", ret);

    /* c. ���ò��� */
    ret = myuvc_set_streaming_params(&myuvc_params);
    printk("myuvc_set_streaming_params ret = %d\n", ret);
    
    myuvc_print_streaming_params(&myuvc_params);

    /* d. ����VideoStreaming Interface��ʹ�õ�setting
     * d.1 ��myuvc_paramsȷ������
     * d.2 ����setting��endpoint�ܴ����wMaxPacketSize
     *     �ҵ�������ô����setting
     */
    /* �ֹ�ȷ��:
     * bandwidth = myuvc_params.dwMaxPayloadTransferSize = 1024
     * �۲�lsusb -v -d 0x1e4e:�Ľ��:
     *                wMaxPacketSize     0x0400  1x 1024 bytes
     * bAlternateSetting       8
     */
    usb_set_interface(myuvc_udev, myuvc_streaming_intf, myuvc_streaming_bAlternateSetting);
    
    /* 2. ��������URB */
    ret = myuvc_alloc_init_urbs();
    if (ret)
        printk("myuvc_alloc_init_urbs err : ret = %d\n", ret);

    /* 3. �ύURB�Խ������� */
	for (i = 0; i < MYUVC_URBS; ++i) {
		if ((ret = usb_submit_urb(myuvc_queue.urb[i], GFP_KERNEL)) < 0) {
			printk("Failed to submit URB %u (%d).\n", i, ret);
			myuvc_uninit_urbs();
			return ret;
		}
	}
    
	return 0;
}

/* A13 APPͨ��poll/selectȷ�������ݺ�, �ѻ���Ӷ�����ȡ����
 * �ο�: uvc_dequeue_buffer
 */
static int myuvc_vidioc_dqbuf(struct file *file, void *priv, struct v4l2_buffer *v4l2_buf)
{
    /* APP�������ݾ�����, ��mainqueue��ȡ�����buffer */

    struct myuvc_buffer *buf;
    int ret = 0;

	if (list_empty(&myuvc_queue.mainqueue)) {
		ret = -EINVAL;
		goto done;
	}
    
	buf = list_first_entry(&myuvc_queue.mainqueue, struct myuvc_buffer, stream);

	switch (buf->state) {
	case VIDEOBUF_ERROR:
		ret = -EIO;
	case VIDEOBUF_DONE:
		buf->state = VIDEOBUF_IDLE;
		break;

	case VIDEOBUF_IDLE:
	case VIDEOBUF_QUEUED:
	case VIDEOBUF_ACTIVE:
	default:
		ret = -EINVAL;
		goto done;
	}

	list_del(&buf->stream);

done:
	return ret;
}

/*
 * A14 ֮ǰ�Ѿ�ͨ��mmapӳ���˻���, APP����ֱ�Ӷ�����
 * A15 �ٴε���myuvc_vidioc_qbuf�ѻ���������
 * A16 poll...
 */

/* A17 ֹͣ 
 * �ο� : uvc_video_enable(video, 0)
 */
static int myuvc_vidioc_streamoff(struct file *file, void *priv, enum v4l2_buf_type t)
{
	struct urb *urb;
	unsigned int i;

    /* 1. kill URB */
	for (i = 0; i < MYUVC_URBS; ++i) {
		if ((urb = myuvc_queue.urb[i]) == NULL)
			continue;
		usb_kill_urb(urb);
	}

    /* 2. free URB */
    myuvc_uninit_urbs();

    /* 3. ����VideoStreaming InterfaceΪsetting 0 */
    usb_set_interface(myuvc_udev, myuvc_streaming_intf, 0);
    
    return 0;
}

    /* Control handling */


/* Extract the bit string specified by mapping->offset and mapping->size
 * from the little-endian data stored at 'data' and return the result as
 * a signed 32bit integer. Sign extension will be performed if the mapping
 * references a signed data type.
 */
static __s32 myuvc_get_le_value(const __u8 *data)
{
    int bits = 16;
    int offset = 0;
    __s32 value = 0;
    __u8 mask;

    data += offset / 8;
    offset &= 7;
    mask = ((1LL << bits) - 1) << offset;

    for (; bits > 0; data++) {
        __u8 byte = *data & mask;
        value |= offset > 0 ? (byte >> offset) : (byte << (-offset));
        bits -= 8 - (offset > 0 ? offset : 0);
        offset -= 8;
        mask = (1 << bits) - 1;
    }

    /* Sign-extend the value if needed. */
    value |= -(value & (1 << (16 - 1)));

    return value;
}

/* Set the bit string specified by mapping->offset and mapping->size
 * in the little-endian data stored at 'data' to the value 'value'.
 */
static void myuvc_set_le_value(__s32 value, __u8 *data)
{
	int bits = 16;
	int offset = 0;
	__u8 mask;

	data += offset / 8;
	offset &= 7;

	for (; bits > 0; data++) {
		mask = ((1LL << bits) - 1) << offset;
		*data = (*data & ~mask) | ((value << offset) & mask);
		value >>= offset ? offset : 8;
		bits -= 8 - offset;
		offset = 0;
	}
}
    


/* �ο�:uvc_query_v4l2_ctrl */    
int myuvc_vidioc_queryctrl (struct file *file, void *fh,
                struct v4l2_queryctrl *ctrl)
{
	__u8 type = USB_TYPE_CLASS | USB_RECIP_INTERFACE;
	unsigned int pipe;
    int ret;
    u8 data[2];

    if (ctrl->id != V4L2_CID_BRIGHTNESS)
        return -EINVAL;
    
	memset(ctrl, 0, sizeof *ctrl);
	ctrl->id   = V4L2_CID_BRIGHTNESS;
	ctrl->type = V4L2_CTRL_TYPE_INTEGER;
	strcpy(ctrl->name, "MyUVC_BRIGHTNESS");
	ctrl->flags = 0;

	pipe = usb_rcvctrlpipe(myuvc_udev, 0);
	type |= USB_DIR_IN;

    /* ����USB����ȷ����Щֵ */
	ret = usb_control_msg(myuvc_udev, pipe, GET_MIN, type, PU_BRIGHTNESS_CONTROL << 8,
			ProcessingUnitID << 8 | myuvc_control_intf, data, 2, 5000);
    if (ret != 2)
        return -EIO;
	ctrl->minimum = myuvc_get_le_value(data);	/* Note signedness */


	ret = usb_control_msg(myuvc_udev, pipe, GET_MAX, type,  PU_BRIGHTNESS_CONTROL << 8,
			ProcessingUnitID << 8 | myuvc_control_intf, data, 2, 5000);
    if (ret != 2)
        return -EIO;
	ctrl->maximum = myuvc_get_le_value(data);	/* Note signedness */

	ret = usb_control_msg(myuvc_udev, pipe, GET_RES, type, PU_BRIGHTNESS_CONTROL << 8,
			 ProcessingUnitID << 8 | myuvc_control_intf, data, 2, 5000);
    if (ret != 2)
        return -EIO;
	ctrl->step = myuvc_get_le_value(data);	/* Note signedness */

	ret = usb_control_msg(myuvc_udev, pipe, GET_DEF, type, PU_BRIGHTNESS_CONTROL << 8,
			ProcessingUnitID << 8 | myuvc_control_intf, data, 2, 5000);
    if (ret != 2)
        return -EIO;
	ctrl->default_value = myuvc_get_le_value(data);	/* Note signedness */

    printk("Brightness: min =%d, max = %d, step = %d, default = %d\n", ctrl->minimum, ctrl->maximum, ctrl->step, ctrl->default_value);
    
    return 0;
}

/* �ο� : uvc_ctrl_get */
int myuvc_vidioc_g_ctrl (struct file *file, void *fh,
                struct v4l2_control *ctrl)
{
	__u8 type = USB_TYPE_CLASS | USB_RECIP_INTERFACE;
	unsigned int pipe;
    int ret;
    u8 data[2];
    
    if (ctrl->id != V4L2_CID_BRIGHTNESS)
        return -EINVAL;

	pipe = usb_rcvctrlpipe(myuvc_udev, 0);
	type |= USB_DIR_IN;

	ret = usb_control_msg(myuvc_udev, pipe, GET_CUR, type, PU_BRIGHTNESS_CONTROL << 8,
			ProcessingUnitID << 8 | myuvc_control_intf, data, 2, 5000);
    if (ret != 2)
        return -EIO;
	ctrl->value = myuvc_get_le_value(data);	/* Note signedness */
    
    return 0;
}

/* �ο�: uvc_ctrl_set/uvc_ctrl_commit */
int myuvc_vidioc_s_ctrl (struct file *file, void *fh,
                struct v4l2_control *ctrl)
{
    __u8 type = USB_TYPE_CLASS | USB_RECIP_INTERFACE;
    unsigned int pipe;
    int ret;
    u8 data[2];
    
    if (ctrl->id != V4L2_CID_BRIGHTNESS)
        return -EINVAL;

    myuvc_set_le_value(ctrl->value, data);

    pipe = usb_sndctrlpipe(myuvc_udev, 0);
    type |= USB_DIR_OUT;

    ret = usb_control_msg(myuvc_udev, pipe, SET_CUR, type, PU_BRIGHTNESS_CONTROL << 8,
            ProcessingUnitID  << 8 | myuvc_control_intf, data, 2, 5000);
    if (ret != 2)
        return -EIO;
    
    return 0;
}



static const struct v4l2_ioctl_ops myuvc_ioctl_ops = {
        // ��ʾ����һ������ͷ�豸
        .vidioc_querycap      = myuvc_vidioc_querycap,

        /* �����о١���á����ԡ���������ͷ�����ݵĸ�ʽ */
        .vidioc_enum_fmt_vid_cap  = myuvc_vidioc_enum_fmt_vid_cap,
        .vidioc_g_fmt_vid_cap     = myuvc_vidioc_g_fmt_vid_cap,
        .vidioc_try_fmt_vid_cap   = myuvc_vidioc_try_fmt_vid_cap,
        .vidioc_s_fmt_vid_cap     = myuvc_vidioc_s_fmt_vid_cap,
        
        /* ����������: ����/��ѯ/�������/ȡ������ */
        .vidioc_reqbufs       = myuvc_vidioc_reqbufs,
        .vidioc_querybuf      = myuvc_vidioc_querybuf,
        .vidioc_qbuf          = myuvc_vidioc_qbuf,
        .vidioc_dqbuf         = myuvc_vidioc_dqbuf,

        /* ��ѯ/���/�������� */
        .vidioc_queryctrl     = myuvc_vidioc_queryctrl,
        .vidioc_g_ctrl        = myuvc_vidioc_g_ctrl,
        .vidioc_s_ctrl        = myuvc_vidioc_s_ctrl,
        
        // ����/ֹͣ
        .vidioc_streamon      = myuvc_vidioc_streamon,
        .vidioc_streamoff     = myuvc_vidioc_streamoff,   
};

/* A1 */
static int myuvc_open(struct file *file)
{
	return 0;
}

static void myuvc_vm_open(struct vm_area_struct *vma)
{
	struct myuvc_buffer *buffer = vma->vm_private_data;
	buffer->vma_use_count++;
}

static void myuvc_vm_close(struct vm_area_struct *vma)
{
	struct myuvc_buffer *buffer = vma->vm_private_data;
	buffer->vma_use_count--;
}

static struct vm_operations_struct myuvc_vm_ops = {
	.open		= myuvc_vm_open,
	.close		= myuvc_vm_close,
};


/* A9 �ѻ���ӳ�䵽APP�Ŀռ�,�Ժ�APP�Ϳ���ֱ�Ӳ�����黺�� 
 * �ο�: uvc_v4l2_mmap
 */
static int myuvc_mmap(struct file *file, struct vm_area_struct *vma)
{
    struct myuvc_buffer *buffer;
    struct page *page;
    unsigned long addr, start, size;
    unsigned int i;
    int ret = 0;

    start = vma->vm_start;
    size = vma->vm_end - vma->vm_start;

    /* Ӧ�ó������mmap����ʱ, �ᴫ��offset����
     * �������offset�ҳ�ָ���Ļ�����
     */
    for (i = 0; i < myuvc_queue.count; ++i) {
        buffer = &myuvc_queue.buffer[i];
        if ((buffer->buf.m.offset >> PAGE_SHIFT) == vma->vm_pgoff)
            break;
    }

    if (i == myuvc_queue.count || size != myuvc_queue.buf_size) {
        ret = -EINVAL;
        goto done;
    }

    /*
     * VM_IO marks the area as being an mmaped region for I/O to a
     * device. It also prevents the region from being core dumped.
     */
    vma->vm_flags |= VM_IO;

    /* ���������ַ�ҵ���������Ӧ��page���� */
    addr = (unsigned long)myuvc_queue.mem + buffer->buf.m.offset;
    while (size > 0) {
        page = vmalloc_to_page((void *)addr);

        /* ��page��APP����������ַ�ҹ� */
        if ((ret = vm_insert_page(vma, start, page)) < 0)
            goto done;

        start += PAGE_SIZE;
        addr += PAGE_SIZE;
        size -= PAGE_SIZE;
    }

    vma->vm_ops = &myuvc_vm_ops;
    vma->vm_private_data = buffer;
    myuvc_vm_open(vma);

done:
    return ret;
}

/* A12 APP����POLL/select��ȷ�������Ƿ����(������) 
 * �ο� : uvc_v4l2_poll
 */
static unsigned int myuvc_poll(struct file *file, struct poll_table_struct *wait)
{
	struct myuvc_buffer *buf;
	unsigned int mask = 0;
    
    /* ��mainqueuq��ȡ����1�������� */

    /*�ж�����״̬, ���δ����, ���� */

    if (list_empty(&myuvc_queue.mainqueue)) {
        mask |= POLLERR;
        goto done;
    }
    
    buf = list_first_entry(&myuvc_queue.mainqueue, struct myuvc_buffer, stream);

    poll_wait(file, &buf->wait, wait);
    if (buf->state == VIDEOBUF_DONE ||
        buf->state == VIDEOBUF_ERROR)
        mask |= POLLIN | POLLRDNORM;
    
done:
    return mask;
}

/* A18 �ر� */
static int myuvc_close(struct file *file)
{
    
	return 0;
}

static const struct v4l2_file_operations myuvc_fops = {
	.owner		= THIS_MODULE,
    .open       = myuvc_open,
    .release    = myuvc_close,
    .mmap       = myuvc_mmap,
    .ioctl      = video_ioctl2, /* V4L2 ioctl handler */
    .poll       = myuvc_poll,
};

static void myuvc_release(struct video_device *vdev)
{
}


static int myuvc_probe(struct usb_interface *intf,
		     const struct usb_device_id *id)
{
    static int cnt = 0;
	struct usb_device *dev = interface_to_usbdev(intf);

    myuvc_udev = dev;

    printk("myuvc_probe : cnt = %d\n", cnt++);

    if (cnt == 1)
    {
        myuvc_control_intf = intf->cur_altsetting->desc.bInterfaceNumber;
    }
    else if (cnt == 2)
    {
        myuvc_streaming_intf = intf->cur_altsetting->desc.bInterfaceNumber;
    }

    if (cnt == 2)
    {
        /* 1. ����һ��video_device�ṹ�� */
        myuvc_vdev = video_device_alloc();

        /* 2. ���� */
        /* 2.1 */
        myuvc_vdev->release = myuvc_release;
        
        /* 2.2 */
        myuvc_vdev->fops    = &myuvc_fops;
        
        /* 2.3 */
        myuvc_vdev->ioctl_ops = &myuvc_ioctl_ops;

        /* 3. ע�� */
        video_register_device(myuvc_vdev, VFL_TYPE_GRABBER, -1);
    }
    
    
    return 0;
}

static void myuvc_disconnect(struct usb_interface *intf)
{
    static int cnt = 0;
    printk("myuvc_disconnect : cnt = %d\n", cnt++);

    if (cnt == 2)
    {
        video_unregister_device(myuvc_vdev);
        video_device_release(myuvc_vdev);
    }
    
}

static struct usb_device_id myuvc_ids[] = {
	/* Generic USB Video Class */
	{ USB_INTERFACE_INFO(USB_CLASS_VIDEO, 1, 0) },  /* VideoControl Interface */
    { USB_INTERFACE_INFO(USB_CLASS_VIDEO, 2, 0) },  /* VideoStreaming Interface */
	{}
};

/* 1. ����usb_driver */
/* 2. ���� */
static struct usb_driver myuvc_driver = {
    .name       = "myuvc",
    .probe      = myuvc_probe,
    .disconnect = myuvc_disconnect,
    .id_table   = myuvc_ids,
};

static int myuvc_init(void)
{
    /* 3. ע�� */
    usb_register(&myuvc_driver);
    return 0;
}

static void myuvc_exit(void)
{
    usb_deregister(&myuvc_driver);
}

module_init(myuvc_init);
module_exit(myuvc_exit);

MODULE_LICENSE("GPL");



