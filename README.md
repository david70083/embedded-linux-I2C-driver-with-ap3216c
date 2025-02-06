# I2C驅動

  ![ ](https://drive.google.com/uc?export=view&id=1ZttNfU4Co61kCbOCD4bNgvVmFIVURg9A)

 - I2C總線驅動:
   
   I2C總線在內核中使用i2c_adapter結構體，由半導體廠來定義後向內核註冊

   ![ ](https://drive.google.com/uc?export=view&id=1kOxmqC5lQRQ4C9tk3bfUx_BTjkuqsdzP)

   ![ ](https://drive.google.com/uc?export=view&id=1RtmkUBQD4cb7-d0KHr-ACSSdbgTJes-Z)

   在i2c_adapter
            -> i2c_algorithm
                            -> master_xfer //I2C最終進行數據傳輸函式
