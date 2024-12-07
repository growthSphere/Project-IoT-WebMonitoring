<?php 
    include("koneksi.php");

    date_default_timezone_set('Asia/Jakarta');
    $waktu = date("H:i:s");
    $tanggal = date("d F Y");
    $air = $_GET ["air"];
    $suhu = $_GET ["suhu"];
    $kelembapan = $_GET ["kelembapan"];
    $cuaca = $_GET ["cuaca"];

    $kirim = mysqli_query($koneksi,"INSERT INTO tbmonitor (waktu,tanggal,air,suhu,kelembapan,cuaca) 
    VALUES ('$waktu','$tanggal','$air','$suhu','$kelembapan','$cuaca')");

    if($kirim == TRUE) {
        echo "Data berhasil diinputkan...!";
    }
    else {
        echo "Gagal di inputkan";
    }
?>  