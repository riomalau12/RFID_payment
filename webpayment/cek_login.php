<?php
include "config/koneksi.php";
$username = $_POST['username'];
$password = md5($_POST['password']);

$query = "SELECT * FROM admin WHERE username = '$username' AND password = '$password'";
$login = mysqli_query($konek, $query);
$ketemu = mysqli_num_rows($login);
$data = mysqli_fetch_array($login);

if($ketemu > 0){
	session_start();

	$_SESSION['namaadmin'] = $data['username'];
	$_SESSION['passadmin'] = $data['password'];

	header("location:menu.php?module=rfid");
}else{
	echo "Login Gagal! Username dan Password tidak benar<br>";
	echo "<a href=\"form_login.php\">Ulangi Lagi</a>";
}
?>