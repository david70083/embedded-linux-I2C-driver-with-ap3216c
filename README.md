# I2C驅動

  ![ ](https://drive.google.com/uc?export=view&id=1ZttNfU4Co61kCbOCD4bNgvVmFIVURg9A)

 - I2C總線驅動:
   
   I2C總線在內核中使用i2c_adapter結構體，由半導體廠來定義後向內核註冊

   ![ ](https://drive.google.com/uc?export=view&id=1kOxmqC5lQRQ4C9tk3bfUx_BTjkuqsdzP)

   ![ ](https://drive.google.com/uc?export=view&id=1RtmkUBQD4cb7-d0KHr-ACSSdbgTJes-Z)

   在i2c_adapter
            -> i2c_algorithm
                            -> master_xfer //I2C最終進行數據傳輸函式


 - I2C設備驅動:

    - I2C設備驅動會調用半導體廠註冊的i2c_adapter結構體來進行I2C傳輸，再針對不同外部設備進行後續應用。
      設備驅動中需要兩個結構: i2c_client用於描述設備訊息和i2c_driver用於描述設備驅動

       - 1.	建立設備驅動結構體(自行定義)
           a.	cdev、class、device、devid、major、minor: 創建字符設備
           b.	private_data: 儲存i2c_client資料
  
        ![ ](https://drive.google.com/uc?export=view&id=1a7YRA7XmLYgqPK9gy9a3bvk5yU-1rpLA)
     
       - 2.	建立i2c_driver結構體:
          a.	.driver: 驅動名稱與.of_match_table匹配表(compatible匹配)
          b.	.probe: 建立匹配成功後要執行的函式
          c.	.remove: 建立關閉匹配後要執行函式
          d.	.id_table: 建立匹配表(name匹配)

