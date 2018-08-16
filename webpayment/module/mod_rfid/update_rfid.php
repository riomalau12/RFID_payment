<?php
session_start();
include "../../config/koneksi.php";
// include "config/fungsi_thumbnail.php";

$id = $_POST['id'];
$nama = $_POST['nama'];
$saldo = $_POST['saldo'];
$nomorrfid = $_POST['nomorrfid'];

// $acak = rand(1, 99);
// $nama_gambar = $acak.$nama_file;

$query = "SELECT * FROM rfid_profile WHERE rfid_id = '$id'";
$hasil = mysqli_query($konek, $query);

if ($hasil) {
	$r = mysqli_fetch_array($hasil);

	$edit = "UPDATE rfid_profile SET name = '$nama',
	balance = '$saldo',
	rfid_number = '$nomorrfid'
	WHERE rfid_id = '$id'";

	$sql = mysqli_query($konek, $edit);
	if ($sql) {
		header("location:../../menu.php?module=rfid");
	}else{
		echo "Gagal mengubah";
	}
}
?>