#include<linux/module.h>
#include<linux/kernel.h>
/*
#include<linux/usb.h>
#include<linux/fs.h>
#include<linux/cdev.h> //character driver
#include <linux/usb/input.h>
#include <linux/buffer_head.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/init.h>
//#include<linux/hid.h>
*/

#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/usb/input.h>
#include <linux/hid.h>
#include <linux/fs.h>
#include <asm/segment.h>
#include <asm/uaccess.h>
#include <linux/buffer_head.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include<linux/semaphore.h>


static char current_data = 0;
struct usb_mouse {
	char name[128];
	char phys[64]; //figure out what this does
	struct usb_device *usbdev;
	struct input_dev *dev;
	struct urb *irq;

	signed char *data;
	dma_addr_t data_dma;

    struct semaphore sem;
    
    char dataforpassing[100];
    
}virtual_device;


//completion handler
static void usb_mouse_irq(struct urb *urb)
{
	struct usb_mouse *mouse = urb->context; //context pointer passes information to completon handler
	signed char *data = mouse->data;
	struct input_dev *dev = mouse->dev;
	int status;

	switch (urb->status) {
	case 0:			/* success */
		break;
	case -ECONNRESET:	/* unlink */ //The urb was unlinked by a call to usb_unlink_urb, and the transfer_flags variable of the urb was set to URB_ASYNC_UNLINK
	case -ENOENT: //The urb was stopped by a call to usb_kill_urb.
	case -ESHUTDOWN: //There was a severe error with the USB host controller driver; it has now been disabled, or the device was disconnected from the system,
		return;
	/* -EPIPE:  should clear the halt */
	default:		/* error */
		goto resubmit;
	}
	
   


	
	input_report_rel(dev, REL_X,     data[1]);
	input_report_rel(dev, REL_Y,     data[3]);
	input_report_rel(dev, REL_WHEEL, data[5]);

	input_sync(dev);

/* to tell those who receive the events that we've sent a complete report.
This doesn't seem important in the one button case, but is quite important
for for example mouse movement, where you don't want the X and Y values
to be interpreted separately*/

	input_report_key(dev, BTN_LEFT,   data[0] & 0x01);
	input_report_key(dev, BTN_RIGHT,  data[0] & 0x02);
	input_report_key(dev, BTN_MIDDLE, data[0] & 0x04);
	input_report_key(dev, BTN_SIDE,   data[0] & 0x08);
	input_report_key(dev, BTN_EXTRA,  data[0] & 0x10);

	



	resubmit:
	status = usb_submit_urb (urb, GFP_ATOMIC); //submit the urb to usb core
	if (status)
		dev_err(&mouse->usbdev->dev,
			"can't resubmit intr, %s-%s/input0, status %d\n",
			mouse->usbdev->bus->bus_name,
			mouse->usbdev->devpath, status);

   
    current_data = data[0];		
	/*if(!(data[0] & 0x01) && !(data[0] & 0x02) && !(data[0]&0x04))
	{
		pr_info("No button pressed!\n");
		return;			//Neither button pressed
	}*/
	
	
		
	//check which button pressed
	if(data[0] & 0x01){
		pr_info("Left mouse button clicked!\n");
		
	}
	else if(data[0] & 0x02){
		pr_info("Right mouse button clicked!\n");
	}
	
  else if(data[0] & 0x04){
		pr_info("middle mouse button clicked!\n");
	}
   
    else if(data[0] & 0x08){
		pr_info("thumb mouse button 2 clicked!\n");
	}
    else if(data[0] & 0x10){
		pr_info("thumb mouse button 1 clicked!\n");
		
	}
    


   
    //write some code to print which button is pressed here


}

//called when input device is opened
static int usb_mouse_open(struct input_dev *dev)
{
    //initializing the urb and mouse structs

	struct usb_mouse *mouse = input_get_drvdata(dev);

	mouse->irq->dev = mouse->usbdev; //irq is the urb associated with mouse and urb struct has a usb_device *dev
	if (usb_submit_urb(mouse->irq, GFP_KERNEL))
		return -EIO;

	return 0;
}
//called when input device is closed
static void usb_mouse_close(struct input_dev *dev)
{
	struct usb_mouse *mouse = input_get_drvdata(dev);

	usb_kill_urb(mouse->irq);
}
static struct usb_device_id mouse_table[]=
{
    /* data */
    //18f8:1485
    //{ USB_DEVICE(0x18f8, 0x1485) },
    { USB_INTERFACE_INFO(USB_INTERFACE_CLASS_HID, USB_INTERFACE_SUBCLASS_BOOT,
	USB_INTERFACE_PROTOCOL_MOUSE) },//check it
    {}//terminating entry


};

struct cdev* mycdev;
int major_number;
dev_t dev_num; //used to hold maj and min no
#define DEVICE_NAME "[Maxxter] USB OPTICAL MOUSE"
int ret;



//inode is reference to file on disk
//struct file represents abstract open file responsibile for containing the fops struct
int device_open(struct inode *node,struct file* filp){
    //only one process should open the file at a time
    if(down_interruptible(&virtual_device.sem)!=0)
    {
    
        printk(KERN_INFO"could not lock device when trying to open device");    

        return -1;
    }

    printk(KERN_INFO"opened the maxterm mouse device");

   
    return 0;


}
int device_close(struct inode *node,struct file* filp){
    //only one process should open the file at a time
    
    up(&virtual_device.sem);
    printk(KERN_INFO"closed the maxterm device");
    return 0;


}

//read method called when user want info from device
//ssize_t is just int from -1 to max
static ssize_t device_read(struct file *filp, char __user *buffer, size_t buffcount, loff_t *curroffset){
    

    //need to take data from kernel to user
    printk(KERN_INFO"reading from device");
    ret = copy_to_user(buffer,&current_data,1);
	current_data = 0;
    return ret;
    //return stores bytes read or written

}

static ssize_t device_write(struct file *filp, const char __user *buffer,size_t buffcount,loff_t *curroffset){
    

    //need to take data from kernel to user
    printk(KERN_INFO"reading from device");
   // printk(KERN_INFO"THIS IS THE BUFFER: %s",buffer);

    ret = copy_from_user(&current_data,buffer,1);
	current_data=0;
    return ret;

}


static struct file_operations fops = {
   //these are file callback functions
    .owner = THIS_MODULE,
    .open = device_open,
    .release = device_close,
    .write = device_write,
    .read = device_read
};


//probe func
static int mouse_probe(struct usb_interface *intf,const struct usb_device_id* id)
{
    /* data */
    printk(KERN_INFO"[*]mouse plugged in (%04X:%04x)PLUGGED IN/n",id->idVendor,id->idProduct);

    /*A USB device driver commonly has to convert data from a given 
    struct usb_interface structure into a struct usb_device structure that the USB core needs for a wide range of function calls
    */
    struct usb_device *dev = interface_to_usbdev(intf);
	struct usb_host_interface *interface; //usb host interface is an array of interfaces
	struct usb_endpoint_descriptor *endpoint;
	struct usb_mouse *mouse;
	struct input_dev *input_dev;
	int pipe, maxp;
	int error = -ENOMEM;
	
	
	interface = intf->cur_altsetting;

	if (interface->desc.bNumEndpoints != 1)
		return -ENODEV;

	endpoint = &interface->endpoint[0].desc;
	if (!usb_endpoint_is_int_in(endpoint))
		return -ENODEV;

	pipe = usb_rcvintpipe(dev, endpoint->bEndpointAddress); //Specifies an interrupt IN endpoint for the specified USB device with the specified endpoint number.
	maxp = usb_maxpacket(dev, pipe);

	mouse = kzalloc(sizeof(struct usb_mouse), GFP_KERNEL); 
    /*while the current process sleeps, the kernel takes proper action to locate some free memory, either by 
    flushing buffers to disk or by swapping out memory from a user process*/
    //is performed on behalf of a process running in kernel space. In other words, this means that the calling function is executing a system call on behalf of a process
	
    
    input_dev = input_allocate_device();
	if (!mouse || !input_dev)
		goto fail1;

   //size of buffer might need to be changed
	mouse->data = usb_alloc_coherent(dev, 8, GFP_ATOMIC, &mouse->data_dma);//allocate dma-consistent buffer
	if (!mouse->data)
		goto fail1;

	mouse->irq = usb_alloc_urb(0, GFP_KERNEL); // first parameter, iso_packets, is the number of isochronous packets this urb should contain
	//isochrnouse pckts r real time data transfers for audio or video purposes
    if (!mouse->irq)
		goto fail2;

	mouse->usbdev = dev;
	mouse->dev = input_dev;

	if (dev->manufacturer)
		strlcpy(mouse->name, dev->manufacturer, sizeof(mouse->name));

	if (dev->product) {
		if (dev->manufacturer)
			strlcat(mouse->name, " ", sizeof(mouse->name));
		strlcat(mouse->name, dev->product, sizeof(mouse->name));
	}

	if (!strlen(mouse->name))
		snprintf(mouse->name, sizeof(mouse->name),
			 "USB HIDBP Mouse %04x:%04x",
			 le16_to_cpu(dev->descriptor.idVendor),
			 le16_to_cpu(dev->descriptor.idProduct)); //

	usb_make_path(dev, mouse->phys, sizeof(mouse->phys));
	strlcat(mouse->phys, "/input0", sizeof(mouse->phys));

	input_dev->name = mouse->name;
	input_dev->phys = mouse->phys;
	
    
    usb_to_input_id(dev, &input_dev->id);
	input_dev->dev.parent = &intf->dev;


//need to make changes here
//  varname =| 2 is same as = varname|2
/*BITS_TO_LONGS(x) - returns the length of a bitfield array in longs for
			   x bits
	BIT_WORD(x)	 - returns the index in the array in longs for bit x
	BIT_MASK(x)	 - returns the index in a long for bit x
*/

	input_dev->evbit[0] = BIT_MASK(EV_KEY) | BIT_MASK(EV_REL);

	input_dev->keybit[BIT_WORD(BTN_MOUSE)] = BIT_MASK(BTN_LEFT) |
		BIT_MASK(BTN_RIGHT) | BIT_MASK(BTN_MIDDLE);
	
    input_dev->relbit[0] = BIT_MASK(REL_X) | BIT_MASK(REL_Y);
	
    input_dev->keybit[BIT_WORD(BTN_MOUSE)] |= BIT_MASK(BTN_SIDE) |
		BIT_MASK(BTN_EXTRA);
	
    input_dev->relbit[0] |= BIT_MASK(REL_WHEEL);

	
    input_set_drvdata(input_dev, mouse);

	input_dev->open = usb_mouse_open;
	input_dev->close = usb_mouse_close;

//to properly initialize a urb to be sent to an interrupt endpoint of a USB device:
	//maxp refers to buffer length might have to make changes here as well
    usb_fill_int_urb(mouse->irq, dev, pipe, mouse->data,
			 (maxp > 8 ? 8 : maxp),
			 usb_mouse_irq, mouse, endpoint->bInterval);
	
    mouse->irq->transfer_dma = mouse->data_dma;
    mouse->irq->transfer_flags |= URB_NO_TRANSFER_DMA_MAP;//Should be set when the urb contains a DMA buffer to be transferred. The USB core uses the buffer pointed to by the transfer_dma variable and not the buffer pointed to by the transfer_buffer variabl

	error = input_register_device(mouse->dev);
	if (error)
		goto fail3;

	
    
    usb_set_intfdata(intf, mouse);
	

	//got to change this
	//register device
    /*
	t = register_chrdev(91, "mymouse", &pugs_fops);
	if(t<0) 
	{
		pr_info("mymouse registration failed\n");
		registered = 0;
	}
	else 
	{
		pr_info("mymouse registration successful\n");
		registered = 1;
	}
	return t;*/
   ret = alloc_chrdev_region(&dev_num,0,1,DEVICE_NAME);
   
    if(ret<0)
    {
    printk(KERN_INFO"failed to allocated major number");
    return ret;

    }
	 mycdev = cdev_alloc();
    mycdev->ops = &fops;
    mycdev->owner = THIS_MODULE;

major_number = MAJOR(dev_num);
    printk(KERN_INFO"make a device file i.e mknod /dev/mymousedev c %d 0\n",major_number);



    ret = cdev_add(mycdev,dev_num,1); //add me to kernel
    if(ret<0)
    {
    printk(KERN_INFO"unable to add chdev");
    return ret;
    }


    sema_init(&virtual_device.sem,1);//initial value of 1, only want one device at a

	return ret;

fail3:	
	usb_free_urb(mouse->irq);
fail2:	
	usb_free_coherent(dev, 8, mouse->data, mouse->data_dma); //might have to change this
fail1:	
	input_free_device(input_dev);
	kfree(mouse);
	return error;
    return 0;
}

//disconnect function
static void mouse_disconnect(struct usb_interface *intf)
{
    /* data */

    printk(KERN_INFO"mouse_disconnect called");


    struct usb_mouse *mouse = usb_get_intfdata(intf);

	usb_set_intfdata(intf, NULL);
	if (mouse) {
		usb_kill_urb(mouse->irq);
		input_unregister_device(mouse->dev);
		usb_free_urb(mouse->irq);
		usb_free_coherent(interface_to_usbdev(intf), 8, mouse->data, mouse->data_dma);
		kfree(mouse);
	}

	cdev_del(mycdev);
    unregister_chrdev_region(dev_num,1);
    
    printk(KERN_INFO"existing driver disconnected");

   
    
}


static struct usb_driver mouse_driver=
{
    .name = "advanced functionality mouse driver",
    .id_table=mouse_table,
    .probe = mouse_probe,
    .disconnect = mouse_disconnect,
};
//usb_deviceid








/*
static int __init mouse_init(void){
    int ret = -1;
    printk(KERN_INFO"constructor of mousedriver  init func is called");
    printk(KERN_INFO"registering driver with kernel");

    ret = usb_register(&mouse_driver);
    printk(KERN_INFO"registeration complete");



   ret = alloc_chrdev_region(&dev_num,0,1,DEVICE_NAME);

    if(ret<0)
    {
    printk(KERN_INFO"failed to allocated major number");
    return ret;

    }
    major_number = MAJOR(dev_num);
    printk(KERN_INFO"make a device file i.e mknod /dev/mymousedev c %d 0\n",major_number);

    printk(KERN_INFO"major number is %d",major_number);
    
    //allocatign char driver and assigning file ops etc
    mycdev = cdev_alloc();
    mycdev->ops = &fops;
    mycdev->owner = THIS_MODULE;

    ret = cdev_add(mycdev,dev_num,1); //add me to kernel
    if(ret<0)
    {
    printk(KERN_INFO"unable to add chdev");
    return ret;
    }

    sema_init(&virtual_device.sem,1);//initial value of 1, only want one device at a time


   

    return ret;


}


static void __exit mouse_exit(void){
    printk(KERN_INFO"destructor of mousedriver da init func");

    usb_deregister(&mouse_driver);
    printk(KERN_INFO"deregisteration complete");

 //need to undo all that we did earlire
    cdev_del(mycdev);
    unregister_chrdev_region(dev_num,1);
    printk(KERN_INFO"unloaded the module");




}*/

MODULE_DEVICE_TABLE (usb, mouse_table);



//module_init(mouse_init);
//module_exit(mouse_exit);
module_usb_driver(mouse_driver);

MODULE_AUTHOR("nirmal");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("nirmals's first driver with GPL licence.");


