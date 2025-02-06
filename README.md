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
           - .	cdev、class、device、devid、major、minor: 創建字符設備
           - .	private_data: 儲存i2c_client資料
  
        ![ ](https://drive.google.com/uc?export=view&id=1a7YRA7XmLYgqPK9gy9a3bvk5yU-1rpLA)
     
       - 2.	建立i2c_driver結構體:
           - .	.driver: 驅動名稱與.of_match_table匹配表(compatible匹配)
           - .	.probe: 建立匹配成功後要執行的函式
           - .	.remove: 建立關閉匹配後要執行函式
           - .	.id_table: 建立匹配表(name匹配)
             
        	 ![ ](https://drive.google.com/uc?export=view&id=1zwT6Kdr7GYPdDT738ouOYQI9yVIhigzU)

          	 ![ ](https://drive.google.com/uc?export=view&id=1_kYrTR3HHoKQGPqwBuhM7AFPcSZoWX1C)

      - 3. 實作.probe函式，創建字符設備
           
      		 ![ ](https://drive.google.com/uc?export=view&id=187Qm587HIwHyGtWB8brAPIDK6Nj9LRgC)

      - 4. 實作i2c讀/寫函式
           
    		 ![ ](https://drive.google.com/uc?export=view&id=1jstn2gajzVXfzsI7VIGISRq8re0xC7h6)

   		 ![ ](https://drive.google.com/uc?export=view&id=1qCb0gEqBKjOA8p_HOGO2I-m_ktpY3er9)


  - I2C設備樹:
      在對應的i2c節點下進行添加
      - i2c引腳復位並使用該腳位設定                 pinctrl-0 = <&pinctrl_i2c2>;
      - 添加外部設備節點，@後為外設地址       ft5426: ft5426@14
      - 添加compatible屬性，供設備驅動匹配用compatible = "david,ft5426";
      - 添加reg地址				          reg = <0x14>;
      - 復位外設所需引腳(pintrl子系統)	          pinctrl-0 = <&pinctrl_tsc
							                                  &pinctrl_tsc_reset>;
      - 設定外設GPIO中斷				interrupt-parent = <&gpio1>;
						                  interrupts = <9 0>;
      - 設定外設使用GPIO子系統		
                                        reset-gpios = <&gpio5 9 GPIO_ACTIVE_LOW>;
                                        interrupt-gpios = <&gpio1 9 GPIO_ACTIVE_LOW>;
        ![ ](https://drive.google.com/uc?export=view&id=1r29yVvwVtUg8tWd3i-tYdtvmo7o9lpuy)
        
        ![ ](https://drive.google.com/uc?export=view&id=1sWCteR22ClambM3-l3pqwvcQm6nkJ9Ix)



        

