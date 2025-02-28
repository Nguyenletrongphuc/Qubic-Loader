def kiemtrasonguyento(n):
    if n <= 1:
        return False
    for i in range(2, int(n** 0.5)+1):
        if n % i ==0:
            return False
    return True
number = int (input("nhap vao so nguyen to can kiem tra"))
if kiemtrasonguyento(number):
    print(number, "la so nguyen to")
else:
    print(number, "khong phai so nguyen to")
                    