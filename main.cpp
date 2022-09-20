
#include <unistd.h>

#include <sys/stat.h>

#include "cableTesting.h"

Serial_Port *UsbPort;
Serial_Port *UartPort;
pthread_t read_usb_tid  = 0;
pthread_t read_uart_tid = 0;
pthread_t button_tid = 0;
pthread_t monitor_tid = 0;
pthread_t write_tid = 0;
int count = 0;

void quit_handler( int sig );
void *start_read_usb_thread(void *args);
void *start_read_uart_thead(void *args);
void *start_button_thread(void *arg);
void *start_monitor_thead(void *args);
void *start_write_thread(void *args);
void serialPort_init(void);

int main(int argc, char *argv[]) 
{
    /// creat monitor thread
    int result = 0;
    
    result = pthread_create(&button_tid, NULL, &start_button_thread, (int *)1);
    result = pthread_create(&monitor_tid, NULL, &start_monitor_thead, (int *)1);

	serialPort_init();

	while(1)
	{

	}
}

// ------------------------------------------------------------------------------
//   Quit Signal Handler
// ------------------------------------------------------------------------------
// this function is called when you press Ctrl-C
void quit_handler(int sig)
{
    printf("\n");
    printf("TERMINATING AT USER REQUEST\n");
    printf("\n");

    // // wait for exit
    // pthread_join(read_usb_tid, NULL);
    // pthread_join(read_uart_tid, NULL);
    // pthread_join(monitor_tid, NULL);
    // pthread_join(write_tid, NULL);

    // serial port
    try {
        UsbPort->handle_quit(sig);
        UartPort->handle_quit(sig);

    } catch (int error) {}

    // end program here
    exit(0);   
}

void serialPort_init(void)
{
	char *usb_name = (char *)"/dev/ttyUSB0";
    char *uart_name = (char *)"/dev/ttyS0";
    int baudrate = 921600;
	int result = 0;

    /*
     * Instantiate a serial port object
     *
     * This object handles the opening and closing of the offboard computer's
     * serial port over which it will communicate to an autopilot.  It has
     * methods to read and write a mavlink_message_t object.  To help with read
     * and write in the context of pthreading, it gaurds port operations with a
     * pthread mutex lock.
     *
     */
    Serial_Port Usb_port(usb_name, baudrate);	
    Serial_Port Uart_port(uart_name, baudrate);	

    /*
     * Setup interrupt signal handler
     *
     * Responds to early exits signaled with Ctrl-C.  The handler will command
     * to exit offboard mode if required, and close threads and the port.
     * The handler in this example needs references to the above objects.
     *
     */
    UsbPort      = &Usb_port;
    UartPort      = &Uart_port;
    signal(SIGINT, quit_handler);

    /*
     * Start the port 
     * This is where the port is opened, and read and write threads are started.
     */
    UsbPort->start();
    UartPort->start();

	// --------------------------------------------------------------------------
	//   CHECK SERIAL PORT
	// --------------------------------------------------------------------------
    if (UsbPort != NULL) {
        if (UsbPort->status != 1) { // SERIAL_PORT_OPEN
            fprintf(stderr, "ERROR: USB port not open\n");
            throw 1;
        }

    } else {
        fprintf(stderr, "ERROR: USB port not exist\n");
        throw 1;
    }

    if (UartPort != NULL) {
        if (UartPort->status != 1) { // SERIAL_PORT_OPEN
            fprintf(stderr, "ERROR: UART port not open\n");
            throw 1;
        }

    } else {
        fprintf(stderr, "ERROR: UART port not exist\n");
        throw 1;
    }
           
    // --------------------------------------------------------------------------
    //   READ THREAD
    // --------------------------------------------------------------------------
 
    result = pthread_create(&read_usb_tid, NULL, &start_read_usb_thread, (int *)1);
    result = pthread_create(&read_uart_tid, NULL, &start_read_uart_thead, (int *)1);
    result = pthread_create(&write_tid, NULL, &start_write_thread, (int *)1);

    cableTesting_init();

    if (result) throw result;

    // now we're reading messages
    printf("\n");
    // serial_port.stop();


}
// ------------------------------------------------------------------------------
//  Pthread Starter Helper Functions
// ------------------------------------------------------------------------------

void *start_read_usb_thread(void *args)
{
    DebugInfo("\nSTART READ USB THREAD !!!\n");

    cableTesting_UsbReadProcess();
    // done!
    return NULL;
}

void *start_read_uart_thead(void *args)
{
    cableTesting_UartReadProcess();
    // done!
    return NULL;
}

void *start_button_thread(void *arg)
{
    cableTesting_manager();

    return NULL;
}

void *start_monitor_thead(void *args)
{
    DebugInfo("\nSTART MONITOR THREAD !!!\n");

    for(;;)
    {
        cableTesting_ledStatus();
    }

    return NULL;
}

void *start_write_thread(void *args)
{
    DebugInfo("START WRITE THREAD !!!");

    cableTesting_SendDataProcess();

    return NULL;
}


