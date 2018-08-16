import processing.serial.*; //import the Serial library
import http.requests.*;

Serial myPort;  //the Serial port object
String val;

void setup() {
  size(200, 200); //make our canvas 200 x 200 pixels big
  //  initialize your serial port and set the baud rate to 9600
  myPort = new Serial(this, "COM4", 9600);
  myPort.bufferUntil('\n');
}

void draw() {
  background(0);
  textSize(22);
  text("Nomor Kartu :"+val, 10, 30); 
  fill(0, 102, 153);
}

void serialEvent( Serial myPort) {
  //put the incoming data into a String - 
  //the '\n' is our end delimiter indicating the end of a complete packet
  val = myPort.readStringUntil('\n');
  //make sure our data isn't empty before continuing
  if (val != null) {
    //trim whitespace and formatting characters (like carriage return)
    val = trim(val);
    println(val);
    PostRequest post = new PostRequest("http://localhost:8080/webpayment/payment.php");
    post.addData("nomorrfid", val);
    post.send();
    System.out.println("Reponse Content: " + post.getContent());
    System.out.println("Reponse Content-Length Header: " + post.getHeader("Content-Length"));
    draw();
    myPort.write(post.getContent() + '~');
  }
}

