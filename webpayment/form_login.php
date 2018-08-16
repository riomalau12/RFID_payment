<html>
<head>
	<title>RFID Profile</title>
	<link rel="icon" href="img/logo.png"/>
    <link rel="shortcut icon" href="img/logo.png" type="image/x-icon" />
	<meta name="viewport" content="width=device-width, initial-scale=1.0"/>
	<link rel="stylesheet" href="css/foundation.min.css">
	<link rel="stylesheet" href="foundation-icons/foundation-icons.css">
	<link rel="stylesheet" href="webicons-master/webicons.css">
	<link rel="stylesheet" href="responsive-tables/responsive-tables.css">
	<script src="js/vendor/modernizr.js"></script>
</head>
<body style="background-color: #EEEEEE">
<div class="row" style="margin-top:120px">
	<div class="columns large-8 large-centered">
		<div class="panel clearfix" style="background:#333333">
			<h3 style="color:white">RFID Profile</h3>
			<hr>
      <form method="POST" action="cek_login.php">
			<div class="row collapse">
				<div class="columns large-2">
					<span class="prefix"><i class="fi-torso-business" style="
					font-size:35px"></i></span>
				</div>
				<div class="columns large-10">
					<input type="text" placeholder="Username" name="username" required>
				</div>
			</div>
			<div class="row collapse">
				<div class="columns large-2">
					<label class="prefix"><i class="fi-key" style="font-size:35px;"></i></label>
				</div>
				<div class="columns large-10">
					<input type="password" placeholder="Password" name="password" required>
				</div>
			</div>
				<input type="reset" value="Ulang" class="button warning right small">
				<input type="submit" class="small green button" value="Login"/>
        </form>
		</div>
	</div>
</div>
<script src="js/vendor/jquery.js"></script>
	<script src="js/foundation.min.js"></script>
	<script src="responsive-tables/responsive-tables.js"></script>
	<script>
	$(document).foundation();
	</script>
</body>
</html>
