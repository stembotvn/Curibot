# Easybot

Easybot là bộ thư viện viết riêng cho Board EasyControl V1, Core Arduino Nano V3 do STEMbot.vn phát triển

STEMbot.vn phát triển bộ thư viện Easybot có thể giúp các bạn mới làm quen với Arduino và Robot tiếp cận dễ dàng hơn bao giờ hết, không còn quan tâm đến các phần cứng, và các chương trình con phức tạp. Code đơn giản, ngắn gọn để các bạn chỉ tập trung vào thuật toán giải quyết các vấn đề. Các sản phẩm dùng Easybot:

- Wibot http://stembot.vn/products/wibot
- RACEbot http://stembot.vn/products/racebot
- Battlebot http://stembot.vn/products/battlebot
## Cài Đặt Thư viện
  Download code từ thư mục này theo định dạng .Zip file. 
  * Trên Phần mềm Arduino IDE: chọn Sketch/Include Library/Add Zip Library -> Chọn đường dẫn đến file .Zip vừa tải về. 
  * Khởi động lại Arduino IDE, chọn Tools/Board/Arduino Nano
  * Vào Files/Examples/Easybot để xem code mẫu
## Lập trình Scratch với phần mềm mBlock
  Easybot có thể lập trình bằng ngôn ngữ Scratch thay cho Arduino:
  https://github.com/stembotvn/Easybot_mBlock_Extension
  Tài liệu hướng dẫn học Easybot với ngôn ngữ Scratch:
  https://github.com/stembotvn/Easybot_mBlock_doccument
## Sử dụng thư viện:
### Khởi tạo 
```
     #include <Easybot.h>
    EasybotNano Robot; // Một đối tượng robot tên là Robot được tạo ra từ lớp EasybotNano
````
### Các hàm di chuyển
```
    Robot.stop(); //dừng robot; 
```
```
    Robot.moveForward(speed); //với speed = 0-100(%) 	điều khiển robot di chuyển đi tới với tốc độ speed (bánh bên phải chạy với tốc độ = speed, bánh trái chạy với tốc độ = speed
```
```
    Robot.moveForward(LeftSpeed,rightSpeed); //với leftSpeed, rightSpeed = 0-100 (%), điều khiển robot đi thẳng với tốc độ bên trái và bên phải tùy chỉnh. 
```
```
    Robot.moveBack(speed); // với speed = 0-100(%) 	điều khiển robot di chuyển đi lùi với tốc độ speed (bánh bên phải chạy với tốc độ = speed, bánh trái chạy với tốc độ = speed
```
```
    Robot.turnLeft(speed); //với speed = 0 - 100(%), điều khiển robot quay trái, tâm quay tại tâm robot, bánh trái quay lùi với tốc độ speed, bánh phải quay tới với tốc độ speed 
```
```
    Robot.turnRight(speed); //với speed = 0 - 100(%), điều khiển robot quay phải, tâm quay tại tâm robot, bánh phải quay lùi với tốc độ speed, bánh trái quay tới với tốc độ speed 
```
```
    Robot.turnLeft(speed,time);// hàm quay trái nâng cao với speed = 0 - 100(%): tham số tốc độ , time = 0 -10 :tham số thời gian-> Quay trái robot với tốc độ = speed trong khoảng thời gian là time x 0.1s
```
```
    Robot.turnRight(speed,time); // hàm quay phải nâng cao với speed = 0 - 100(%): tham số tốc độ , time = 0 -10 :tham số thời gian-> Quay phải robot với tốc độ = speed trong khoảng thời gian là time x 0.1s 		
```
### Các hàm đọc các cảm biến đi theo combo của Wibot (cảm biến dò line và cảm biến siêu âm đo khoảng cách)
``` 
    int value = Robot.readSensor(Channel);  //gán giá trị Analog đọc từ sensor vào biến value kiểu Interger, Channel = LEFTSENSOR,CENTERSENSOR, RIGHTSENSOR

    Serial.println(value); //xuất giá trị analog từ Sensor ra màn hình PC (Từ Arduino IDE vào Tools/Serial Monitor)
```
```
bool result = Robot.leftSensor(); // Trả về giá trị Logic là 0 hay 1 (True or False) cho cảm biến dò line trái (0: không có vạch, 1: có vạch)
```
```
Serial.print(Robot.leftSensor()); //xuất giá trị kết quả (1 or 0) phân tích từ analog Sensor, ngưỡng mặc định so sánh để phát hiện vạch kẻ là 400
```   
```   
 int value = Robot.readSonar(); //Đọc cảm biến siêu âm SRF04 gán giá trị đo khoảng cách tính bằng Cm vào biến Value kiểu Interger
```	
 
    
  
  
