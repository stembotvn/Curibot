# Curibot

Curibot là bộ thư viện firmware dành cho bộ kit Curie STEMkit giúp cho các em học sinh từ 10 tuổi có thể dễ dàng làm quen với lắp ráp, lập trình Robot đơn giản với ngôn ngữ Scratch 2.0. 


## Tính năng chính của board mạch Curie trên robot Curibot
- 2 Động cơ DC
- Buzzer với thư viện âm thanh phong phú 
- RGB mixing: trộn màu đèn LED RGB 
- 2 cảm biến ánh sáng trái phải (giúp robot có thể định hướng theo ánh sáng) 
- Cảm biến siêu âm SRF04 để đo khoảng cách
- Cảm biến dò line 3 vị trí giúp robot bám vạch chính xác
- Động cơ Servo để mở rộng các cơ cấu khác. 
- Kết nối Wireless với Chip NRF24L01 của Nordic giúp Robot dễ dàng tương tác với máy tính qua ngôn ngữ lập trình Scratch
- Đồng bộ địa chỉ tự động(pairing giữa robot và card USB wireless NEGENDO) giúp việc thiết lập kết nối nhanh chóng và dễ dàng, không phải cài đặt địa chỉ
- Cổng mở rộng 4 chân (Digital/Analogs) để kết nối với các Module Add-on, mở rộng các ứng dụng và bài học để đặt bước chân đầu tiên vào thế giới kỹ thuật số với Arduino dễ dàng. 

## Chế độ hoạt động
Robot được nạp sẵn firmware cho phép người dùng dễ dàng chuyển đổi các chế độ hoặt động thông qua nút SET trên board mạch, các chế độ có màu từ đèn LED để phân biệt:
1 - Lập trình Scratch (đồng bộ với chương trình Scratch MIT tiêu chuẩn) thông qua kết nối không dây hoặc có dây
2 - Chạy chương trình đã nạp xuống robot từ phần mềm mBlock (cho phép người dùng viết chương trình Scratch và lưu ở bộ xử lý của robot chứ không phụ thuộc kết nối máy tính và phần mềm Scratch)
3 - Chế độ tránh vật cản
4 - Chế độ dò line
5 - Chế độ đi theo ánh sáng 

Với tính năng dễ dàng chuyển đổi các chế độ, người dùng sẽ chuyển đổi cách thức sử dụng robot linh hoạt mà không cần phải nạp đi nạp lại các chương trình Arduino, khi cần nạp chương trình Arduino để giải quyết bài toán nào đó, chương trình này sẽ lưu tại một vùng nhớ và khởi chạy khi chuyển chế độ bằng nút bấm vào chế độ 2, các tính năng và chế độ khác không bị ảnh hưởng bởi việc nạp chương trình mới. 



  
