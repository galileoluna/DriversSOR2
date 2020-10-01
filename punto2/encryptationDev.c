/*
 * chardev.c: Creates a read−only char device that says how many times
 * you've read from the dev file
 */
#include <linux/ctype.h>  // isalpha, isupper
#include <linux/string.h> // strcspns
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h> 

static int device_open (struct inode *, struct file *) ;
static int device_release (struct inode *, struct file *) ;
static ssize_t device_read (struct file *, char *, size_t , loff_t *) ;
static ssize_t device_write (struct file *, const char *, size_t , loff_t *);

#define DEVICE_NAME "encryptationDev"
#define BUF_LEN 80 /* Max length of the message from the device */
/*
 * Global variables are declared as static, so are global within the file.
 */
static int Major;
static int Device_Open = 0;
static char msj[BUF_LEN];
static char *msg_Ptr;
static char *msg_PtrAux;

//Encrypation Variables

#define LONGITUD_ALFABETO 26
#define INICIO_ALFABETO_MAYUSCULAS 65
#define INICIO_ALFABETO_MINUSCULAS 97


#define MOD(i, n) (i % n + n) % n // Calcular módulo positivo, gracias a

const char *alfabetoMinusculas = "abcdefghijklmnopqrstuvwxyz",
           *alfabetoMayusculas = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
// Recibe un mensaje a cifrar y un búfer en donde pondrá el resultado, así como
// las rotaciones que debe dar
void cifrar(char *mensaje, char *destino);

// Obtener el valor entero de un carácter:
int ord(char c);

static struct file_operations fops = {
 .read = device_read,
 .write = device_write,
 .open = device_open,
 .release = device_release
};

int init_module ( void ){ 
    /* Constructor */
    Major = register_chrdev(0, DEVICE_NAME, &fops);
    if (Major < 0) {
        printk(KERN_ALERT  "Registering char device failed with %d\n", Major);
        return Major;
    }
            printk(KERN_INFO "I was assigned major number %d. To talk to\n", Major);
            printk(KERN_INFO "the driver, create a dev file with\n");
            printk(KERN_INFO "'mknod /dev/%s c %d 0'.\n", DEVICE_NAME, Major);
            printk(KERN_INFO "Try various minor numbers. Try to cat and echo to\n");
            printk(KERN_INFO "the device file.\n");
            printk(KERN_INFO "Remove the device file and module when done.\n");
    return 0;
}
/*
 * This function is called when the module is unloaded
 */
void cleanup_module ( void ){
    unregister_chrdev(Major, DEVICE_NAME);
    printk ( KERN_INFO "Driver unregister \n") ;
}

static int device_open(struct inode *inode, struct file *file){
    if (Device_Open) return -EBUSY;
    Device_Open++;
    try_module_get(THIS_MODULE);
    return 0;
}
/*
 * Called when a process closes the device file.
 */
static int device_release(struct inode *inode, struct file *file){
    Device_Open--; 
    module_put(THIS_MODULE);
    return 0;
}
/*
 * Called when a process, which already opened the dev file, attempts to
 *  * read from it.
 */

static ssize_t device_read(struct file *filp, char *buffer, size_t length, loff_t * offset){
    int bytes_read = 0;
    if (*msg_Ptr == 0)return 0;
    while (length && *msg_Ptr) {
        put_user(*(msg_Ptr++), buffer++);
        length--;
        bytes_read++;
    }
    /*
    * Most read functions return the number of bytes put into the buffer
    */
    return bytes_read;
}
/*
 * Called when a process writes to dev file: echo "hi" > /dev/hello
 */
static ssize_t device_write(struct file *filp, const char *buff, size_t len, loff_t * off){
        int i;
        for (i = 0; i < len && i < BUF_LEN; i++){
            get_user(msj[i], buff + i);
        }
        msj[len] = '\0'; 
        msg_PtrAux = msj;
        
        cifrar(msj, msg_PtrAux);

        msg_Ptr = msg_PtrAux;
        printk(KERN_ALERT "Encryptation is finished\n");
        return i;
}
void cifrar(char *mensaje, char *destino) {
  /*Recorrer cadena*/
  int i = 0;
  while (mensaje[i]) {
    char caracterActual = mensaje[i];
    int posicionOriginal = ord(caracterActual);
    if (!isalpha(caracterActual)) {
      destino[i] = caracterActual;
      i++;
      continue; // Ir a la siguiente iteración; por eso arriba aumentamos a i
    }
    if (isupper(caracterActual)) {
      destino[i] =
          alfabetoMayusculas[(posicionOriginal - INICIO_ALFABETO_MAYUSCULAS +
                              1) %
                             LONGITUD_ALFABETO];
    } else {

      destino[i] =
          alfabetoMinusculas[(posicionOriginal - INICIO_ALFABETO_MINUSCULAS +
                              1) %
                             LONGITUD_ALFABETO];
    }
    i++;
  }
}

int ord(char c) { return (int)c; }


MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Device Driver");
MODULE_AUTHOR("Galileo Luna ");
