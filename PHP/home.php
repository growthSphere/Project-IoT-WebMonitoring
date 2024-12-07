<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>Monitoring Banjir</title>
    <link rel="stylesheet" type="text/css" href="style.css">
</head>
<body>
    <header class="header" align="center">
        <div class="header-content">
            <img src="logo_umyFix.png" alt="logo umy" class="logo">
            <div class="header-text">
                <h1>MONITORING BANJIR</h1>
                <p>Website ini dapat memantau ketinggian air beserta informasi suhu dan kondisi cuaca saat air naik</p>
            </div>
        </div>
    </header>

    <?php
        include("koneksi.php");

        $query = mysqli_query($koneksi, "SELECT * FROM tbmonitor ORDER BY id DESC LIMIT 1");
        while ($data = mysqli_fetch_array($query)) {
    ?>
    <div class="container">
        <div class="kotak">
            <h2 class="h2">KONDISI AIR</h2>
            <div class="nilai">
            <?php echo $data['air'] ?>
            </div>
        </div>
        <div class="kotak">
            <h2 class="h2">SUHU</h2>
            <div class="nilai">
            <?php echo $data['suhu'] ?><font size="6">°C</font>
            </div>
        </div>
        <div class="kotak">
            <h2 class="h2">KELEMBAPAN</h2>
            <div class="nilai">
            <?php echo $data['kelembapan'] ?><font size="6">%</font>
            </div>
        </div>
        <div class="kotak">
            <h2 class="h2">KONDISI CUACA</h2>
            <div class="nilai">
            <?php echo $data['cuaca'] ?>
            </div>
        </div>
    </div>

    <footer class="footer">
        <img src="footer.png" alt="Footer Image" class="footer-image">
        <p>&copy; 2024 Monitoring Banjir. By. Iqbal Al Akram.</p>
    </footer>
    <?php } ?>
</body>
</html>
