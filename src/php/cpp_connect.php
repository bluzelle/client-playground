

<?php	
  dl("libdb.so");	// Load the module
  require "libdb.php";
  echo "Hello World!";
  $bar = new DB;
  $bar->slowGet(1000);
  echo "Hello World!2";
?>