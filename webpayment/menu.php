<!DOCTYPE html>
<html>
<head>
  <meta charset="utf-8" />
  <meta name="viewport" content="width=device-width, initial-scale=1.0" />
  <title>RFID Profile</title>
  <link rel="icon" href="img/logo.png"/>
  <link rel="shortcut icon" href="img/logo.png" type="image/x-icon" />
  <link rel="stylesheet" href="css/foundation.css" />
  <script src="js/vendor/modernizr.js"></script>
</head>
<body>

  <?php
session_start();
if(empty($_SESSION['namaadmin']) AND empty($_SESSION['passadmin'])){
  echo "Untuk masuk, Anda harus login dahulu. <br>";
  echo "<a href=\"/web/form_login.php\"><b>LOGIN</b></a>";
}
else{
?>

  <div class="off-canvas-wrap" data-offcanvas>
    <div class="inner-wrap">

    <nav class="tab-bar show-for-small">
      <section class="tab-bar-section">
        <img src="" style="margin-left:50px">
      </section>
      <section class="left-small">
        <a class="left-off-canvas-toggle menu-icon"
        href="#"><span></span></a>
      </section>
    </nav>

    <!-- top bar -->
  <div class="sticky">
  <nav class="top-bar hide-for-small" data-topbar role="navigation">
    <ul class="title-area">
      <li class="name">
        <h1><a class="#">RFID Profile</a></h1>
      </li>
      <!-- <li class="toggle-topbar menu-icon">
        <a href="#"><span>Menu</span></a>
      </li> -->
    </ul>
    <section class="top-bar-section">
      <ul class="right">
        <li class="active"><a href="logout.php">Logout</a></li>
      </ul>
    </section>
  </nav>
</div>

<aside class="left-off-canvas-menu">
  <ul class="off-canvas-list">
    <!-- style="background:#008CBA" -->
    <li><a href="menu.php?module=rfid"><img src="img/item_48.png">RFID</a></li>
	<li><a href="logout.php"><img src="img/out_50.png"> Log Out</a></li>
  </ul>
</aside>


<section class="main-section">
  <aside class="columns medium-3 large-2 hide-for-small"
  style="position:fixed; padding:0; height:100%">
  <!-- <div class="hide-for-small-only"> -->
  <div class="icon-bar three-up vertical">

    <a class="item" href="menu.php?module=rfid">
      <img src="img/item_48.png">
      <label>RFID</label>
    </a>
  </div>

</aside>

<content class="columns medium-9 medium-offset-3 large-10 large-offser-2">
  <div style="min-height:400px">

    <?php
      if($_GET['module']=='rfid'){
        include "module/mod_rfid/tampil_rfid.php";
      }

      elseif ($_GET['module']=='form_rfid') {
        include "module/mod_rfid/form_rfid.php";
      }elseif ($_GET['module']=='input_rfid') {
        include "module/mod_rfid/input_rfid.php";
      }elseif ($_GET['module']=='edit_rfid') {
        include "module/mod_rfid/edit_rfid.php";
      }elseif ($_GET['module']=='update_rfid') {
        include "module/mod_rfid/update_rfid.php";
      }elseif ($_GET['module']=='hapus_rfid') {
        include "module/mod_rfid/hapus_rfid.php";
      }
    ?>

  <!--   <div class="row" style="margin-top:20px">
    	<div class="columns large-12">
    		<h3><i class="fi-page-edit"></i>&nbsp; Tambah Pahlawan</h3>
    	</div>
    </div>

    <div class="row">
    <div class="columns large-12">

      <! <div id="messages">
        <span id="message-filler"></span>
      </div> 

      <! form
    <form id="image-form" action="#" data-abide>
    <table width="100%">
    	<tr>
    		<td>Nama</td>
    		<td>
    			<div class="columns medium-6">
    				<input type="text" id="judulInput" placeholder="Judul" required>
            <small class="error">Nama harus diisi</small>
    			</div>
    		</td>
    	</tr>
    	<tr>
    		<td>Deskripsi</td>
    		<td>
    			<div class="columns medium-6">
    				<input type="text" id="informasiInput" placeholder="Informasi" required>
            <small class="error">Deskripsi harus diisi</small>
    			</div>
    		</td>
    	</tr>
      <tr>
        <td></td>
        <td>
          <div class="columns medium-6">
            <input id="mediaCapture" type="file" hidden accept="image/*,capture=camera">
            <small class="error">Gambar harus ditambah</small>
          </div>
        </td>
      </tr>
    </table>

    <div class="row">
      <div class="columns large-7 medium-12">
    	<input type="reset" value="Reset" class="button small right warning">
      <! <input type="submit" value="Tambah" class="button right small" style="margin-right:50px">
      <button id="submitImage" enabled type="submit" title="Add an image">
        Send
      </button>
</div>
</div
    </form>
    </div>
    </div> -->

<!-- <table>
  <thead>
    <tr>
      <th>Nama</th>
      <th>Kalori</th>
      <th>Karbohidrat</th>
      <th>Protein</th>
      <th>Lemak</th>
      <th>Kualitas</th>
    </tr>
  </thead>
</table> -->

</div>
<div style="clear:both"></div>

<footer>
  <hr>
  <p align="center"></p>
</footer>
</content>

</section>
<a class="exit-off-canvas"></a>
</div>
</div>
<!-- </div> -->

<?php
}
?>

<script src="js/vendor/jquery.js"></script>
<script src="js/foundation.min.js"></script>
<script>
  $(document).foundation();
</script>

</body>
</html>
