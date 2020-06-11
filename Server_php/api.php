<?php
/*
 * Umbrella Rental System
 * Copyright 2020 a5768549
 *
 * Server PHP
 * This code responsible for Umbrella Rental System api
 * 
 * GitHub Project：https://github.com/a5768549/Umbrella-Rental-System
 */
date_default_timezone_set("Asia/Taipei");
include('connect.php');

$type   = $_POST['type'];
$device = $_POST['device'];
$action = $_POST['action'];
$data   = $_POST['data'];

$borrow_user_path = "S1/borrow_user.txt";
$return_user_path = "S1/return_user.txt";

//type B is borrow require
if($type == 'B')
{
	switch($action)
	{
		case 'Card_inquire':
			$sql = "SELECT * FROM `member` where `uuid` = ?";
			$result =$con->prepare($sql);
			$result->execute(array($data));
			$result2 = $result->fetch(PDO::FETCH_ASSOC);
			
			if($result2['uuid'] != "")
			{
				echo "1";
			}
			else
			{
				echo "0";
			}
			
		break;
		
		case 'User_check':
			$sql      = "SELECT * FROM `um_data` where `user` = ?";
			$result   = $con->prepare($sql);
			$result  -> execute(array($data));
			$result2  = $result->fetch(PDO::FETCH_ASSOC);
			
			if($result2['uuid'] != "")
			{
				echo "1";
			}
			else
			{
				echo "0";
			}
		break;
		
		case 'Upload_data':
			$sql    =  "Update `member` set `status` = 1 where `uuid` = ?";
			$result =  $con->prepare($sql);
			$result -> execute(array($data));

			$file = fopen($borrow_user_path,"w"); //開啟檔案
			fwrite($file,$data);
			fclose($file);
			
			echo 'success!';
		break;
		
		case 'Check_sb_borrow':
			$file = fopen($borrow_user_path, "r");
			$str = fgets($file);
			fclose($file);
			
			if($str != "no")
			{
				echo "1";
			}
			else
			{
				echo "0";
			}
		break;
		
		case 'Upload_um_data':
			$file = fopen($borrow_user_path, "r");
			$str = fgets($file);
			fclose($file);
			
			$sql = "SELECT * FROM `um_data` where `uuid` = ?";
			$result =$con->prepare($sql);
			$result->execute(array($data));
			$result2 = $result->fetch(PDO::FETCH_ASSOC);
			
			if($result2['uuid'] != "")
			{
				$time = date("Y-m-d H:i:s");
				$sql = "Update `um_data` set `user` = ? where `uuid` = ?";
				$result =$con->prepare($sql);
				$result->execute(array($str,$data));
				
				$sql = "Update `um_data` set `time` = ? where `uuid` = ?";
				$result =$con->prepare($sql);
				$result->execute(array($time,$data));
				
				$file = fopen("S1/borrow_user.txt","w"); //開啟檔案
				fwrite($file,"no");
				fclose($file);
				
				echo "success";
			}
			else
			{
				echo "wrong um";
			}
			
			
		break;
	}
}

//type R is return require
if($type == 'R')
{
	switch($action)
	{
		case 'Upload_um_data':
			$file = fopen($return_user_path,"w"); //開啟檔案
			fwrite($file,$data);
			fclose($file);
		break;
		
		case 'Check_sb_return':
			$file = fopen($return_user_path, "r");
			$str = fgets($file);
			fclose($file);
			
			if($str != "no")
			{
				echo $str;
			}
			else
			{
				echo "no";
			}
		break;
		
		case 'Check_card':
			$file = fopen($return_user_path, "r");
			$str = fgets($file);
			fclose($file);
			
			$sql = "SELECT * FROM `um_data` where `uuid` = ?";
			$result =$con->prepare($sql);
			$result->execute(array($str));
			$result2 = $result->fetch(PDO::FETCH_ASSOC);
			
			if($result2['user'] == $data)
			{
				echo "1";
			}
			else
			{
				echo "0";
			}
		break;
		
		
		case 'Check_card':
			$file = fopen($return_user_path, "r");
			$str = fgets($file);
			fclose($file);
			
			$sql = "SELECT * FROM `um_data` where `uuid` = ?";
			$result =$con->prepare($sql);
			$result->execute(array($str));
			$result2 = $result->fetch(PDO::FETCH_ASSOC);
			
			if($result2['user'] == $data)
			{
				echo "1";
			}
			else
			{
				echo "0";
			}
		break;
		
		case 'Use_money':
			$sql = "SELECT * FROM `um_data` where `uuid` = ?";
			$result =$con->prepare($sql);
			$result->execute(array($str));
			$result2 = $result->fetch(PDO::FETCH_ASSOC);
			
			$last_time = $result2['time'];
			$now_time = date("Y-m-d H:i:s");
			$use_time = round((strtotime($now_time) - strtotime($last_time))/3600);
			$money = $use_time * 5;
			echo $money;
		break;
		
		case 'Upload_data':
			$file = fopen($return_user_path, "r");
			$str = fgets($file);
			fclose($file);
			
			$time = "0000-00-00 00:00:00";
			$user = "no";
			
			
			$sql = "SELECT * FROM `um_data` where `uuid` = ?";
			$result =$con->prepare($sql);
			$result->execute(array($str));
			$result2 = $result->fetch(PDO::FETCH_ASSOC);
			
			$user_uuid = $result2['user'];
			
			$sql = "Update `member` set `status` = 0 where `uuid` = ?";
			$result =$con->prepare($sql);
			$result->execute(array($user_uuid));
			$sql = "Update `um_data` set `time` = ? where `uuid` = ?";
			$result =$con->prepare($sql);
			$result->execute(array($time,$str));
			
			$sql = "Update `um_data` set `user` = ? where `uuid` = ?";
			$result =$con->prepare($sql);
			$result->execute(array($user,$str));
			
			
			
			$file = fopen($return_user_path,"w"); //開啟檔案
			fwrite($file,"no");
			fclose($file);
			
			echo "success";
		break;
	}
}
?>