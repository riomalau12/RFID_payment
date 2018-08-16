<?php
session_start();
include "../../config/koneksi.php";
// include "../../config/fungsi_thumbnail.php";

$nama = $_POST['nama'];
$saldo = $_POST['saldo'];
$nomorrfid = $_POST['nomorrfid'];


	$input = "INSERT INTO rfid_profile(name, balance, rfid_number) VALUES ('$nama', '$saldo', '$nomorrfid')";

	$sql = mysqli_query($konek, $input);

	if ($sql) {
		header("location:../../menu.php?module=rfid");
	}else{
		echo "Menyimpan di database gagal.";
	}
?>