# RFID_payment


1. Download processing pilih versi 2.2.1 di
https://processing.org/download/

2. Buka kode  serialpostproc.pde di processing ide.
3. Copy folder httprequest_processing ke Documents/Processing/libraries.
4. Buka kode serialrfid3.ino di arduino ide.
5. Tancapkan Arduino dan upload kode serialrfid.ino, jangan buka serial monitor.
6. Pada kode processing  myPort = new Serial(this, "COM31", 9600); ganti COM31 dengan port arduino
   yang bisa dilihat pada arduino ide, tools->port.
7. Tekan tombol play di processing.
8. Tempelkan kartu satu kali dan lihat di balance webnya, balance akan berkurang 1.
