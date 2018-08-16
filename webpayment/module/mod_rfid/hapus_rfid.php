<?php
session_start();
include "config/koneksi.php";

$query = "SELECT * FROM rfid_profile WHERE rfid_id = '$_GET[id]'";
$hasil = mysqli_query($konek, $query);

if ($hasil) {

	$r = mysqli_fetch_array($hasil);

	$hapus = "DELETE FROM rfid_profile WHERE rfid_id = '$_GET[id]'";

	$hasil = mysqli_query($konek, $hapus);


	if ($hasil) {
		header("location:menu.php?module=rfid");
	}else{
		echo "Gagal menghapus";
	}

}else{
	echo "Gagal Membaca";
}

?>