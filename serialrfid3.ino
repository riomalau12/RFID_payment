#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>                 // Sketsa ini menggunakan pustaka Keypad.h
#include <SPI.h>                    // Sketsa ini menggunakan pustaka SPI.h
#include <MFRC522.h>                // Sketsa ini menggunakan pustaka MFRC522.h

#define SS_PIN 10                   // Tentukan SS_PIN dari RC522 RFID Reader ke pin digital 10 dari Arduino
#define RST_PIN 9                   // Tentukan RST_PIN dari Pembaca RFID RC522 ke pin digital 9 dari Arduino
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Buat instance MFRC522.

int RedLed    = A0;                 // Output LED merah adalah pin analog A3
int GreenLed  = A1;                 // Output LED hijau adalah pin analog A4
int Buzzer    = A2;                 // Output Buzzer adalah pin analog A5
unsigned long Timer;                // Variabel untuk Timer



const byte ROWS = 4;                // Empat baris
const byte COLS = 3;                // Tiga kolom


char Keys[ROWS][COLS] = {           // Tentukan simbol-simbol pada tombol-tombol keypad
  {'1', '2', '3',},
  {'4', '5', '6',},
  {'7', '8', '9',},
  {'*', '0', '#',}
};

byte rowPins[ROWS] = {5, 4, 3, 2};  // Hubungkan ke pinouts baris keypad
byte colPins[COLS] = {8, 7, 6};     // Hubungkan ke pinouts kolom keypad
Keypad keypad = Keypad( makeKeymap(Keys), rowPins, colPins, ROWS, COLS); // Inisialisasi instance dari kelas NewKeypad

int RightCard;                      // Variabel untuk Kartu Mifare yang diketahui
int RightPinCode;                   // Variabel untuk input kunci yang valid dalam kombinasi dengan Kartu Mifare yang dikenal
int WrongPinCode;                   // Variabel untuk input kunci yang tidak valid dalam kombinasi dengan Kartu Mifare yang diketahui
int PinCodeCounter;                 // Variable Counter untuk menghitung jumlah input kode PIN pada keypad

int Code1Correct;                   // Variabel untuk input digit pertama yang benar (Kode) pada keypad
int Code2Correct;                   // Variable for the second correct digit (Code) input on the keypad
int Code3Correct;                   // Variable for the third correct digit (Code) input on the keypad
int Code4Correct;                   // Variable for the fourth correct digit (Code) input on the keypad
int Code5Correct;                   // Variable for the fifth correct digit (Code) input on the keypad
int Code6Correct;                   // Variable for the sixth correct digit (Code) input on the keypad
int Reset;                          // Setel ulang untuk pengulangan

int MifareCard1;                               // Kami membutuhkan variabel khusus ini untuk menentukan PinCode yang valid dalam kombinasi dengan UID nummer dari Kartu Mifare
const int Code1MifareCard1 = '4';              // Tentukan di sini digit pertama dari PIN 6 digit Anda
const int Code2MifareCard1 = '4';              // Determine here the second digit of your 6 digit PIN
const int Code3MifareCard1 = '4';              // Determine here the third digit of your 6 digit PIN
const int Code4MifareCard1 = '4';              // Determine here the fourth digit of your 6 digit PIN
const int Code5MifareCard1 = '4';              // Determine here the fifth digit of your 6 digit PIN
const int Code6MifareCard1 = '4';              // Determine here the sixth digit of your 6 digit PIN

int MifareCard2;                               // Kami membutuhkan variabel khusus ini untuk menentukan PinCode yang valid dalam kombinasi dengan UID nummer dari Kartu Mifare
const int Code1MifareCard2 = '4';              // Tentukan di sini digit pertama dari PIN 6 digit Anda
const int Code2MifareCard2 = '4';              // Determine here the second digit of your 6 digit PIN
const int Code3MifareCard2 = '4';              // Determine here the third digit of your 6 digit PIN
const int Code4MifareCard2 = '4';              // Determine here the fourth digit of your 6 digit PIN
const int Code5MifareCard2 = '4';              // Determine here the fifth digit of your 6 digit PIN
const int Code6MifareCard2 = '4';              // Determine here the sixth digit of your 6 digit PIN


//i2c pins
LiquidCrystal_I2C lcd(0x27, 16, 2);


String inputString = "";
boolean stringComplete = false;
int nomorRFID;
boolean stopRead = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  lcd.begin();
  lcd.backlight();//Power on the back light
  lcd.setCursor(0, 0);
  lcd.println("Selamat Datang  ");
  lcd.setCursor(0, 1);
  lcd.println("Tag Kartu Anda  ");

  SPI.begin();                                            // Initialize the SPI bus
  mfrc522.PCD_Init();                                     // Initialize the MFRC522



  pinMode (RedLed, OUTPUT);                               // Define RedLed as OUTPUT
  pinMode (GreenLed, OUTPUT);                             // Define GreenLed as OUTPUT
  pinMode (Buzzer, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  //  rfidRead();
    serialEvent();
  //  inputString = "1";
  if (stringComplete) {
    //        stopRead = true;
    //    Serial.println(inputString);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Sisa Kuota");
    lcd.setCursor(0, 1);
    lcd.print(inputString);
    inputString = "";
    lcd.print(" Liter");
    delay (3000);
    lcd.setCursor(0, 0);
  lcd.println("Selamat Datang  ");
  lcd.setCursor(0, 1);
  lcd.println("Tag Kartu Anda  ");

 
    stringComplete = false;
    Reset = 1;
  }

  //----------------------------------------------Kode untuk fungsi reset dari loop--------------------------------------------------------------------------------------------------------


  if (Reset == 1)                                           // Jika Reset memiliki nilai 1 (HIGH) semua variabel akan direset ke 0 dan pembaca RFID RC522 akan menunggu Kartu Mifare baru.
  {
    RightCard = 0;
    MifareCard1 = 0;
    MifareCard2 = 0;
    RightPinCode = 0;
    WrongPinCode = 0;
    Code1Correct = 0;
    Code2Correct = 0;
    Code3Correct = 0;
    Code4Correct = 0;
    Code5Correct = 0;
    Code6Correct = 0;
    PinCodeCounter = 0;
    delay (50);
    Reset = 0;
  }


  //----------------------------------------------kode untuk kontrol untuk kode pin yang valid dalam batas waktu 7 detik setelah menampilkan / menawarkan Kartu Mifare yang valid ---------------------------


  if (millis() - Timer > 15000 && RightCard == 1)           // Jika Timer akan lebih besar dalam 7 detik dan Kartu Mifare yang valid ditawarkan, nilai reset akan tinggi.
  { // Ini berarti bahwa Anda memiliki kartu yang valid setelah menyajikan, 7 detik untuk memasukkan kode yang valid atau loop ulang.
    Reset = 1;
    //    Serial.println("CardAccesOff");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Card Access Off");
  }




  //----------------------------------------------Membaca Kartu Mifare yang disajikan / ditawarkan di MFRC522 -----------------------------------------------------------------------------------------


  if   (mfrc522.PICC_IsNewCardPresent() &&
        mfrc522.PICC_ReadCardSerial())
  {


    //----------------------------------------------Kode untuk mendeteksi Kartu Mifare pertama di Basis Data. Ini adalah nomor UID dengan 4 pasang digit. Contoh: 69 7C 01 9C------------------------------------
    if
    (mfrc522.uid.uidByte[0] == 0xA6   &&                // Isi angka pertama dari nomor UID kartu Mifare Anda
        mfrc522.uid.uidByte[1] == 0x8C   &&                // Fill in the seconds digits of the UID number of your Mifare card
        mfrc522.uid.uidByte[2] == 0x97   &&                // Fill in the third digits of the UID number of your Mifare card
        mfrc522.uid.uidByte[3] == 0x25)                   // Fill in the fourth digits of the UID number of your Mifare card

    {
      RightCard = 1;                                     // Kartu Mifare yang ditawarkan dikenal dalam database, atur variabel "RightCard" menjadi 1
      MifareCard1 = 1;                                   // Kartu Mifare ini dikenal sebagai Mifare Card 2 di Database, jadi tetapkan variabel MifareCard2 ke nilai 1.
      digitalWrite (Buzzer, HIGH);                       // Buat bunyi bip untuk Kartu Mifare yang diterima
      delay (150);                                       //
      digitalWrite (Buzzer, LOW);                        //

      PinCodeCounter = 0;                                // Setel ulang PinCodeCounter ke 0
      Timer =  millis();                                 // Setel ulang Timer. Batas 7 detik sedang berjalan sekarang untuk PinCode yang valid
      //      Serial.println("Selamat Datang SARTIKA LESTARI");
      //      Serial.println("Kartu di terima, silahkan masukkan password Anda");                 // Cetak teks "CardAccesOn" ke monitor serial
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Hai Tika");
      lcd.setCursor(0, 1);
      lcd.print("Masukkan password");
      delay (200);                                       // Tunggu 200 milidetik
    }


    //-----------------------------------------------Kode untuk mendeteksi Kartu Mifare kedua di Basis Data. Ini adalah nomor UID dengan 4 pasang digit. Contoh: EB 70 C0 BC----------------------------------------------
    if
    (mfrc522.uid.uidByte[0] == 0x02   &&                 // Isi angka pertama dari nomor UID kartu Mifare Anda
        mfrc522.uid.uidByte[1] == 0xB0   &&                 // Fill in the seconds digits of the UID number of your Mifare card
        mfrc522.uid.uidByte[2] == 0xD6   &&                 // Fill in the third digits of the UID number of your Mifare card
        mfrc522.uid.uidByte[3] == 0x0E)                     // Fill in the fourth digits of the UID number of your Mifare card

    {
      RightCard = 1;                                       // If the offered Mifare Card is known in the database, set variable "RightCard" to 1
      MifareCard2 = 1;
      digitalWrite (Buzzer, HIGH);                         // Make a beep for the accepted Mifare Card
      delay (150);                                         //
      digitalWrite (Buzzer, LOW);                          //

      PinCodeCounter = 0;                                  // Reset the PinCodeCounter to 0
      Timer =  millis();                                   // Reset the Timer. The 7 seconds limit is running now for a valid PinCode
      //      Serial.println("Selamat Datang IKE ARDIANTI TANJUNG");
      //      Serial.println("Kartu di terima, silahkan masukkan password Anda");                   // Print the text "CardAccesOn" to the serial monitor
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Hallo Ike");
      lcd.setCursor(0, 1);
      lcd.print("Masukkan password");
      delay (200);                                         // Wait 200 milliseconds
    }

  }

  //-----------------------------------------------Kode untuk menerima MifareCard + PinCode valid yang valid-------------------------------------------------------------------------------------------------



  if (Code6Correct == 1 && RightCard == 1)                    // Jika PinCode benar dan Anda memasukkan ini dalam 7 detik
  {
    RightPinCode = 1;                                       // Variabel RightPinCode akan diatur ke 1

    digitalWrite (GreenLed, HIGH);                          // Buat bunyi bip yang bagus untuk Pincode yang benar dan atur LED Hijau tinggi selama 1 detik
    delay (150);                                            //
    digitalWrite (Buzzer, HIGH);                            //
    delay (150);                                            //
    digitalWrite (Buzzer, LOW);                             //
    delay (50);                                             //
    digitalWrite (Buzzer, HIGH);                            //
    delay (150);                                            //
    digitalWrite (Buzzer, LOW);                             //
    delay (500);                                            //
    digitalWrite (GreenLed, LOW);                           //

    //    Serial.println("Selamat Datang");
    //    Serial.println("Sisa saldo anda = L");                       // Cetak teks "Correct PinCode" ke monitor serial
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Password Benar");
    delay (2000);
//    Serial.print(nomorRFID);
//    Serial.println("@");
    Serial.println(nomorRFID);
    Reset = 1;                                              // Setel ulang pengulangan

  }


  //-----------------------------------------------Kode untuk menolak MifareCard yang valid + PinCode yang tidak valid------------------------------------------------------------------------------------------------



  if ((Code6Correct == 0) && (PinCodeCounter >= 6) && (RightCard == 1))       //Jika Anda menawarkan Kartu Mifare yang valid dan Pincode tidak benar dan Anda sudah memasukkan 6 kunci digit
  {
    WrongPinCode = 1;                                                       // Variable WrongPinCode akan diatur ke 1
    //    Serial.println("Salah kode");                                            // Cetak teks "WrongKey" ke monitor serial
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Salah Kode");
    Reset = 1;                                                              // Setel ulang pengulangan
  }


  //-----------------------------------------------Kode untuk menolak PinCode Salah / Tidak Valid atau Timer (7 detik) kedaluwarsa----------------------------------------------------------------------------



  if ((WrongPinCode == 1) || (millis() - Timer > 15000 && RightCard == 1))     // Jika Anda menawarkan Kartu Mifare yang valid dan Anda memasukkan PinCode yang salah atau penghitung waktu (7 detik) berakhir
  {
    digitalWrite (Buzzer, HIGH);                                            // Buat bunyi bip panjang dan atur LED Merah TINGGI untuk kode yang salah atau waktu kedaluwarsa
    digitalWrite (RedLed, HIGH);                                            //
    delay(1500);                                                            //
    digitalWrite (Buzzer, LOW);                                             //
    digitalWrite (RedLed, LOW);                                             //

    //    Serial.println("Waktu Habis");                           // Cetak teks "WrongCode or Timer expired" ke monitor serial
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Waktu Habis");
    Reset = 1;                                                              // Setel ulang pengulangan
  }


  //-----------------------------------------------Kode untuk menghitung input pada keypad----------------------------------------------------------------------------------------------------------



  char KeyDigit = keypad.getKey();                                           // Dapatkan DigitKey dari keypad

  if ((RightCard == 1) &&                                                    // Jika Anda menawarkan Kartu Mifare yang valid dan DigitKey apa pun pada keypad ditekan
      ((KeyDigit == '1') ||
       (KeyDigit == '2')  ||
       (KeyDigit == '3')  ||
       (KeyDigit == '4')  ||
       (KeyDigit == '5')  ||
       (KeyDigit == '6')  ||
       (KeyDigit == '7')  ||
       (KeyDigit == '8')  ||
       (KeyDigit == '9')  ||
       (KeyDigit == '0')  ||
       (KeyDigit == '*')  ||
       (KeyDigit == '#')))

  {
    PinCodeCounter++;                                                       // PinCodeCounter memberi nilai +1 untuk setiap penekanan pada tombol Digit di keypad
    digitalWrite (Buzzer, HIGH);                                            // Buat bunyi bip pendek untuk menekan DigitKey pada keypad
    delay (50);                                                             //
    digitalWrite (Buzzer, LOW);                                             //
  }





  //-----------------------------------------------kode untuk menentukan PinCode yang benar untuk MifareCard1----------------------------------------------------------------------------------------------------------





  if ((KeyDigit == Code1MifareCard1) && (RightCard == 1) && (Code1Correct == 0) && (MifareCard1 == 1))           // If you offered the valid MifareCard1, first PinCode and you did not entered it before (Otherwise we conflict with same KeyDigits)
  {
    Code1Correct = 1;                                                                                        // Variable Code1Correct is set to 1
    return;                                                                                                  // Return to begin loop
  }

  if ((KeyDigit == Code2MifareCard1) && (Code1Correct == 1) && (Code2Correct == 0) && (MifareCard1 == 1))        // If you offered the valid MifareCard1, second PinCode and you did not entered it before (Otherwise we conflict with same KeyDigits)
  {
    Code2Correct = 1;                                                                                        // Variable Code2Correct is set to 1
    return;                                                                                                  // Return to begin loop
  }

  if ((KeyDigit == Code3MifareCard1) && (Code2Correct == 1) && (Code3Correct == 0) && (MifareCard1 == 1))         // If you offered the valid MifareCard1, third PinCode and you did not entered it before (Otherwise we conflict with same KeyDigits)
  {
    Code3Correct = 1;                                                                                         // Variable Code3Correct is set to 1
    return;                                                                                                   // Return to begin loop
  }

  if ((KeyDigit == Code4MifareCard1) && (Code3Correct == 1) && (Code4Correct == 0) && (MifareCard1 == 1))         // If you offered the valid MifareCard1, fourth PinCode and you did not entered it before (Otherwise we conflict with same KeyDigits)
  {
    Code4Correct = 1;                                                                                         // Variable Code4Correct is set to 1
    return;                                                                                                   // Return to begin loop
  }
  if ((KeyDigit == Code5MifareCard1) && (Code4Correct == 1) && (Code5Correct == 0) && (MifareCard1 == 1))         // If you offered the valid MifareCard1, fifth PinCode and you did not entered it before (Otherwise we conflict with same KeyDigits)
  {
    Code5Correct = 1;                                                                                         // Variable Code5Correct is set to 1
    return;                                                                                                   // Return to begin loop
  }

  if ((KeyDigit == Code6MifareCard1) && (Code5Correct == 1) && (Code6Correct == 0) && (MifareCard1 == 1))         // If you offered the valid MifareCard1, sixth PinCode and you did not entered it before (Otherwise we conflict with same KeyDigits)
  {
    Code6Correct = 1;
    nomorRFID = 1; //Memasukkan nomor RFID// Variable Code6Correct is set to 1
    return;                                                                                                   // Return to begin loop
  }




  //-----------------------------------------------Code for determine the correct PinCode for MifareCard2----------------------------------------------------------------------------------------------------------





  if ((KeyDigit == Code1MifareCard2) && (RightCard == 1) && (Code1Correct == 0) && (MifareCard2 == 1))              // If you offered a the valid MifareCard2, first PinCode and you did not entered it before (Otherwise we conflict with same KeyDigits)
  {
    Code1Correct = 1;                                                                                           // Variable Code1Correct is set to 1
    return;                                                                                                     // Return to begin loop
  }

  if ((KeyDigit == Code2MifareCard2) && (Code1Correct == 1) && (Code2Correct == 0) && (MifareCard2 == 1))           // If you offered a the valid MifareCard2, second PinCode and you did not entered it before (Otherwise we conflict with same KeyDigits)
  {
    Code2Correct = 1;                                                                                           // Variable Code2Correct is set to 1
    return;                                                                                                     // Return to begin loop
  }

  if ((KeyDigit == Code3MifareCard2) && (Code2Correct == 1) && (Code3Correct == 0) && (MifareCard2 == 1))           // If you offered a the valid MifareCard2, third PinCode and you did not entered it before (Otherwise we conflict with same KeyDigits)
  {
    Code3Correct = 1;                                                                                           // Variable Code3Correct is set to 1
    return;                                                                                                     // Return to begin loop
  }

  if ((KeyDigit == Code4MifareCard2) && (Code3Correct == 1) && (Code4Correct == 0) && (MifareCard2 == 1))          // If you offered a the valid MifareCard2, fourth PinCode and you did not entered it before (Otherwise we conflict with same KeyDigits)
  {
    Code4Correct = 1;                                                                                          // Variable Code4Correct is set to 1
    return;                                                                                                    // Return to begin loop
  }
  if ((KeyDigit == Code5MifareCard2) && (Code4Correct == 1) &&  (Code5Correct == 0) && (MifareCard2 == 1))         // If you offered a the valid MifareCard2, fifth PinCode and you did not entered it before (Otherwise we conflict with same KeyDigits)
  {
    Code5Correct = 1;                                                                                          // Variable Code5Correct is set to 1
    return;                                                                                                    // Return to begin loop
  }

  if ((KeyDigit == Code6MifareCard2) && (Code5Correct == 1) &&  (Code6Correct == 0) && (MifareCard2 == 1))         // If you offered a the valid MifareCard2, sixth PinCode and you did not entered it before (Otherwise we conflict with same KeyDigits)
  {
    Code6Correct = 1;                                                                                          // Variable Code6Correct is set to 1
    nomorRFID = 2; //Memasukkan nomor RFID
    return;                                                                                                    // Return to begin loop
  }
}


void serialEvent() {
  while (Serial.available()) {
    //get th new byte
    char inChar = (char)Serial.read();
    //add it to the input string
 
    if (inChar == '~') {
      stringComplete = true;
      return;
    }
    inputString += inChar;
  }
}


//void rfidRead() {
//  //----------------------------------------------Kode untuk fungsi reset dari loop--------------------------------------------------------------------------------------------------------
//  if (Reset == 1)                                           // Jika Reset memiliki nilai 1 (HIGH) semua variabel akan direset ke 0 dan pembaca RFID RC522 akan menunggu Kartu Mifare baru.
//  {
//    RightCard = 0;
//    MifareCard1 = 0;
//    MifareCard2 = 0;
//    RightPinCode = 0;
//    WrongPinCode = 0;
//    Code1Correct = 0;
//    Code2Correct = 0;
//    Code3Correct = 0;
//    Code4Correct = 0;
//    Code5Correct = 0;
//    Code6Correct = 0;
//    PinCodeCounter = 0;
//    delay (50);
//    Reset = 0;
//  }
//
//
//  //----------------------------------------------kode untuk kontrol untuk kode pin yang valid dalam batas waktu 7 detik setelah menampilkan / menawarkan Kartu Mifare yang valid ---------------------------
//
//
//  if (millis() - Timer > 15000 && RightCard == 1)           // Jika Timer akan lebih besar dalam 7 detik dan Kartu Mifare yang valid ditawarkan, nilai reset akan tinggi.
//  { // Ini berarti bahwa Anda memiliki kartu yang valid setelah menyajikan, 7 detik untuk memasukkan kode yang valid atau loop ulang.
//    Reset = 1;
//    //    Serial.println("CardAccesOff");
//  }
//
//
//
//
//  //----------------------------------------------Membaca Kartu Mifare yang disajikan / ditawarkan di MFRC522 -----------------------------------------------------------------------------------------
//
//
//  if   (mfrc522.PICC_IsNewCardPresent() &&
//        mfrc522.PICC_ReadCardSerial())
//  {
//
//
//    //----------------------------------------------Kode untuk mendeteksi Kartu Mifare pertama di Basis Data. Ini adalah nomor UID dengan 4 pasang digit. Contoh: 69 7C 01 9C------------------------------------
//    if
//    (mfrc522.uid.uidByte[0] == 0x32   &&                // Isi angka pertama dari nomor UID kartu Mifare Anda
//        mfrc522.uid.uidByte[1] == 0xF5   &&                // Fill in the seconds digits of the UID number of your Mifare card
//        mfrc522.uid.uidByte[2] == 0x3C   &&                // Fill in the third digits of the UID number of your Mifare card
//        mfrc522.uid.uidByte[3] == 0x2F)                    // Fill in the fourth digits of the UID number of your Mifare card
//
//    {
////      stopRead = false;
//      RightCard = 1;                                     // Kartu Mifare yang ditawarkan dikenal dalam database, atur variabel "RightCard" menjadi 1
//      MifareCard1 = 1;                                   // Kartu Mifare ini dikenal sebagai Mifare Card 2 di Database, jadi tetapkan variabel MifareCard2 ke nilai 1.
//      digitalWrite (Buzzer, HIGH);                       // Buat bunyi bip untuk Kartu Mifare yang diterima
//      delay (150);                                       //
//      digitalWrite (Buzzer, LOW);                        //
//
//      PinCodeCounter = 0;                                // Setel ulang PinCodeCounter ke 0
//      Timer =  millis();                                 // Setel ulang Timer. Batas 7 detik sedang berjalan sekarang untuk PinCode yang valid
//      //      Serial.println("Selamat Datang SARTIKA LESTARI");
//      //      Serial.println("Kartu di terima, silahkan masukkan password Anda");                 // Cetak teks "CardAccesOn" ke monitor serial
//      delay (200);                                       // Tunggu 200 milidetik
//    }
//
//
//    //-----------------------------------------------Kode untuk mendeteksi Kartu Mifare kedua di Basis Data. Ini adalah nomor UID dengan 4 pasang digit. Contoh: EB 70 C0 BC----------------------------------------------
//    if
//    (mfrc522.uid.uidByte[0] == 0x32   &&                 // Isi angka pertama dari nomor UID kartu Mifare Anda
//        mfrc522.uid.uidByte[1] == 0x09   &&                 // Fill in the seconds digits of the UID number of your Mifare card
//        mfrc522.uid.uidByte[2] == 0xF3   &&                 // Fill in the third digits of the UID number of your Mifare card
//        mfrc522.uid.uidByte[3] == 0x2F)                     // Fill in the fourth digits of the UID number of your Mifare card
//
//    {
////      stopRead = false;
//      RightCard = 1;                                       // If the offered Mifare Card is known in the database, set variable "RightCard" to 1
//      MifareCard2 = 1;
//      digitalWrite (Buzzer, HIGH);                         // Make a beep for the accepted Mifare Card
//      delay (150);                                         //
//      digitalWrite (Buzzer, LOW);                          //
//
//      PinCodeCounter = 0;                                  // Reset the PinCodeCounter to 0
//      Timer =  millis();                                   // Reset the Timer. The 7 seconds limit is running now for a valid PinCode
//      //      Serial.println("Selamat Datang IKE ARDIANTI TANJUNG");
//      //      Serial.println("Kartu di terima, silahkan masukkan password Anda");                   // Print the text "CardAccesOn" to the serial monitor
//      delay (200);                                         // Wait 200 milliseconds
//    }
//
//  }
//
//  rfidProcess();
//
//  //-----------------------------------------------Kode untuk menolak MifareCard yang valid + PinCode yang tidak valid------------------------------------------------------------------------------------------------
//
//
//
//  if ((Code6Correct == 0) && (PinCodeCounter >= 6) && (RightCard == 1))       //Jika Anda menawarkan Kartu Mifare yang valid dan Pincode tidak benar dan Anda sudah memasukkan 6 kunci digit
//  {
//    WrongPinCode = 1;                                                       // Variable WrongPinCode akan diatur ke 1
//    //    Serial.println("Salah kode");                                            // Cetak teks "WrongKey" ke monitor serial
//    Reset = 1;                                                              // Setel ulang pengulangan
//  }
//
//
//  //-----------------------------------------------Kode untuk menolak PinCode Salah / Tidak Valid atau Timer (7 detik) kedaluwarsa----------------------------------------------------------------------------
//
//
//
//  if ((WrongPinCode == 1) || (millis() - Timer > 7000 && RightCard == 1))     // Jika Anda menawarkan Kartu Mifare yang valid dan Anda memasukkan PinCode yang salah atau penghitung waktu (7 detik) berakhir
//  {
//    digitalWrite (Buzzer, HIGH);                                            // Buat bunyi bip panjang dan atur LED Merah TINGGI untuk kode yang salah atau waktu kedaluwarsa
//    digitalWrite (RedLed, HIGH);                                            //
//    delay(1500);                                                            //
//    digitalWrite (Buzzer, LOW);                                             //
//    digitalWrite (RedLed, LOW);                                             //
//
//    //    Serial.println("Waktu Habis");                           // Cetak teks "WrongCode or Timer expired" ke monitor serial
//    Reset = 1;                                                              // Setel ulang pengulangan
//  }
//
//
//  //-----------------------------------------------Kode untuk menghitung input pada keypad----------------------------------------------------------------------------------------------------------
//
//
//
//  char KeyDigit = keypad.getKey();                                           // Dapatkan DigitKey dari keypad
//
//  if ((RightCard == 1) &&                                                    // Jika Anda menawarkan Kartu Mifare yang valid dan DigitKey apa pun pada keypad ditekan
//      ((KeyDigit == '1') ||
//       (KeyDigit == '2')  ||
//       (KeyDigit == '3')  ||
//       (KeyDigit == '4')  ||
//       (KeyDigit == '5')  ||
//       (KeyDigit == '6')  ||
//       (KeyDigit == '7')  ||
//       (KeyDigit == '8')  ||
//       (KeyDigit == '9')  ||
//       (KeyDigit == '0')  ||
//       (KeyDigit == '*')  ||
//       (KeyDigit == '#')))
//
//  {
//    PinCodeCounter++;                                                       // PinCodeCounter memberi nilai +1 untuk setiap penekanan pada tombol Digit di keypad
//    digitalWrite (Buzzer, HIGH);                                            // Buat bunyi bip pendek untuk menekan DigitKey pada keypad
//    delay (50);                                                             //
//    digitalWrite (Buzzer, LOW);                                             //
//  }
//
//
//
//
//
//  //-----------------------------------------------kode untuk menentukan PinCode yang benar untuk MifareCard1----------------------------------------------------------------------------------------------------------
//
//
//
//
//
//  if ((KeyDigit == Code1MifareCard1) && (RightCard == 1) && (Code1Correct == 0) && (MifareCard1 == 1))           // If you offered the valid MifareCard1, first PinCode and you did not entered it before (Otherwise we conflict with same KeyDigits)
//  {
//    Code1Correct = 1;                                                                                        // Variable Code1Correct is set to 1
//    return;                                                                                                  // Return to begin loop
//  }
//
//  if ((KeyDigit == Code2MifareCard1) && (Code1Correct == 1) && (Code2Correct == 0) && (MifareCard1 == 1))        // If you offered the valid MifareCard1, second PinCode and you did not entered it before (Otherwise we conflict with same KeyDigits)
//  {
//    Code2Correct = 1;                                                                                        // Variable Code2Correct is set to 1
//    return;                                                                                                  // Return to begin loop
//  }
//
//  if ((KeyDigit == Code3MifareCard1) && (Code2Correct == 1) && (Code3Correct == 0) && (MifareCard1 == 1))         // If you offered the valid MifareCard1, third PinCode and you did not entered it before (Otherwise we conflict with same KeyDigits)
//  {
//    Code3Correct = 1;                                                                                         // Variable Code3Correct is set to 1
//    return;                                                                                                   // Return to begin loop
//  }
//
//  if ((KeyDigit == Code4MifareCard1) && (Code3Correct == 1) && (Code4Correct == 0) && (MifareCard1 == 1))         // If you offered the valid MifareCard1, fourth PinCode and you did not entered it before (Otherwise we conflict with same KeyDigits)
//  {
//    Code4Correct = 1;                                                                                         // Variable Code4Correct is set to 1
//    return;                                                                                                   // Return to begin loop
//  }
//  if ((KeyDigit == Code5MifareCard1) && (Code4Correct == 1) && (Code5Correct == 0) && (MifareCard1 == 1))         // If you offered the valid MifareCard1, fifth PinCode and you did not entered it before (Otherwise we conflict with same KeyDigits)
//  {
//    Code5Correct = 1;                                                                                         // Variable Code5Correct is set to 1
//    return;                                                                                                   // Return to begin loop
//  }
//
//  if ((KeyDigit == Code6MifareCard1) && (Code5Correct == 1) && (Code6Correct == 0) && (MifareCard1 == 1))         // If you offered the valid MifareCard1, sixth PinCode and you did not entered it before (Otherwise we conflict with same KeyDigits)
//  {
//    Code6Correct = 1;                                                                                         // Variable Code6Correct is set to 1
//    nomorRFID = 1; //Memasukkan nomor RFID
//    return;                                                                                                   // Return to begin loop
//  }
//
//
//
//
//  //-----------------------------------------------Code for determine the correct PinCode for MifareCard2----------------------------------------------------------------------------------------------------------
//
//
//
//
//
//  if ((KeyDigit == Code1MifareCard2) && (RightCard == 1) && (Code1Correct == 0) && (MifareCard2 == 1))              // If you offered a the valid MifareCard2, first PinCode and you did not entered it before (Otherwise we conflict with same KeyDigits)
//  {
//    Code1Correct = 1;                                                                                           // Variable Code1Correct is set to 1
//    return;                                                                                                     // Return to begin loop
//  }
//
//  if ((KeyDigit == Code2MifareCard2) && (Code1Correct == 1) && (Code2Correct == 0) && (MifareCard2 == 1))           // If you offered a the valid MifareCard2, second PinCode and you did not entered it before (Otherwise we conflict with same KeyDigits)
//  {
//    Code2Correct = 1;                                                                                           // Variable Code2Correct is set to 1
//    return;                                                                                                     // Return to begin loop
//  }
//
//  if ((KeyDigit == Code3MifareCard2) && (Code2Correct == 1) && (Code3Correct == 0) && (MifareCard2 == 1))           // If you offered a the valid MifareCard2, third PinCode and you did not entered it before (Otherwise we conflict with same KeyDigits)
//  {
//    Code3Correct = 1;                                                                                           // Variable Code3Correct is set to 1
//    return;                                                                                                     // Return to begin loop
//  }
//
//  if ((KeyDigit == Code4MifareCard2) && (Code3Correct == 1) && (Code4Correct == 0) && (MifareCard2 == 1))          // If you offered a the valid MifareCard2, fourth PinCode and you did not entered it before (Otherwise we conflict with same KeyDigits)
//  {
//    Code4Correct = 1;                                                                                          // Variable Code4Correct is set to 1
//    return;                                                                                                    // Return to begin loop
//  }
//  if ((KeyDigit == Code5MifareCard2) && (Code4Correct == 1) &&  (Code5Correct == 0) && (MifareCard2 == 1))         // If you offered a the valid MifareCard2, fifth PinCode and you did not entered it before (Otherwise we conflict with same KeyDigits)
//  {
//    Code5Correct = 1;                                                                                          // Variable Code5Correct is set to 1
//    return;                                                                                                    // Return to begin loop
//  }
//
//  if ((KeyDigit == Code6MifareCard2) && (Code5Correct == 1) &&  (Code6Correct == 0) && (MifareCard2 == 1))         // If you offered a the valid MifareCard2, sixth PinCode and you did not entered it before (Otherwise we conflict with same KeyDigits)
//  {
//    Code6Correct = 1;                                                                                          // Variable Code6Correct is set to 1
//    nomorRFID = 2; //Memasukkan nomor RFID
//    return;                                                                                                    // Return to begin loop
//  }
//}
//
//void rfidProcess() {
//  //-----------------------------------------------Kode untuk menerima MifareCard + PinCode valid yang valid-------------------------------------------------------------------------------------------------
//
//  if (Code6Correct == 1 && RightCard == 1)                    // Jika PinCode benar dan Anda memasukkan ini dalam 7 detik
//  {
//    RightPinCode = 1;                                       // Variabel RightPinCode akan diatur ke 1
//
//    digitalWrite (GreenLed, HIGH);                          // Buat bunyi bip yang bagus untuk Pincode yang benar dan atur LED Hijau tinggi selama 1 detik
//    delay (150);                                            //
//    digitalWrite (Buzzer, HIGH);                            //
//    delay (150);                                            //
//    digitalWrite (Buzzer, LOW);                             //
//    delay (50);                                             //
//    digitalWrite (Buzzer, HIGH);                            //
//    delay (150);                                            //
//    digitalWrite (Buzzer, LOW);                             //
//    delay (500);                                            //
//    digitalWrite (GreenLed, LOW);                           //
//
//
//    Serial.println(nomorRFID + '@');
//    //    Serial.println("Selamat Datang");
//    //    Serial.println("Sisa saldo anda = L");                       // Cetak teks "Correct PinCode" ke monitor serial
//    //    Reset = 1;                                              // Setel ulang pengulangan
//  }
//}

