# ���ؿ���

## ��ʵ�ֵĹ���

1. ��ʱ�Զ�����ͨ�Ϲ㲥֡
2. ��ʱ�Զ������迹�㲥֡
3. ���������л��㲥֡����
4. ��������֡��ת��������

# �ڵ㿪��

## ��ʵ�ֵĹ���

1. ͨ��lora���ܹ㲥֡�ͷ�������֡
2. ���������ͨ�ϼ��
3. ����������迹���
4. ���ɹ㲥֡Զ�����ýڵ�ļ��ģʽ���������ϵͳ�нڵ������������������

## TODO

## ʹ�õ�������

### ADC

ADC��ģ��ת�������ڱ���Ŀ�б�ʹ�ã����ڼ���������ڵ��м���������迹ֵ�����÷�������ѭ��

### DMA

DMA��ֱ���ڴ���ʣ����ڱ���Ŀ�е����ݴ��䵽���ڣ�ԭ���Ǳ���Ŀʵʱ��Ҫ��ϸߣ�ʹ����ѭ���ͽ��п��ܵ��´���ʧ�ܣ�������������ж��쳣

### EXTI

EXTI���ⲿ�жϣ������ڱ���Ŀ�н�������ͨ�ϼ�⣬GD32F470VK����22���ⲿ�жϣ�Ŀǰʹ��12���ⲿ�жϽ����ⲿ�ߵ�ƽ�ж�ʵ��ͨ�ϼ��

### I2C

I2C�ڱ���Ŀ�б�ʹ�ã���������˿���չ��XL9555����ͨѶ

### PMU

PMU�ڱ���Ŀ�б�������оƬ�����ܺĿ��ƣ����˯��ģʽ����Ϊ����Ŀ��ʹ�õ�ص�ϵͳ������������

### TIMER

��ʱ���ڱ���Ŀ�б����ã�����Ϊϵͳʱ��ͬ���Ĺؼ�

### USART

����DMA���ͺʹ����жϽ����ڱ���Ŀ�б�ʹ�ã���������Ϊ�շ��Ĺؼ�

### USB

USB����ʹ�ã�Ŀǰ��δ����

### RTOS

�п��ܻ���ʹ�õ�ʵʱ����ϵͳ����Ҫ��������

## ���̼�¼

### ��չ��·����

����Ҫ�����Ƕ���չ��·������֤

### �������뿪�ؿ���Ƶ�ι���



### ������ϵͳ��ֲ
#### ��ֲ��¼
- ��ֲEXTI done
- ��ֲ֡���� done
- ��ֲͨ�ϼ�� done
- ����һ�ι㲥֡������������ done
- �ɿ�����ӿ�תΪ���̰�ӿ� done
- ͨ�ϲ��� done
    ʵ���ϣ��Ծɱ��ж����ȼ���Ӱ�죬�����ж�Ϊ0��EXTIΪ1����ʱ��Ϊ2����������Ҫ��֤���Ż�

- ��Թ�������ʹ�õ�������FLAG��Ӧͳһ����ΪRTOS�е��ź�����ʽ done
- ��ֲI2C
    I2C�ж���ֲʧ�ܣ�I2C�ж��ƺ��������ϵͳ�г�ͻ���������Ϊ��ѯϵͳ���������ǿ��еģ��ҿɲ����Ը��ߣ���ϵͳ�е�I2Cû�и���ӦҪ��
    ��Ȼ����I2C�жϣ����������ź�������ϵͳ��ϵͳ���������շ������ܵ�Ӱ��
- ������Ƶ���л�Ϊ240M
    �л��󣬾������ԣ�����Ƶ����120M��Ŀǰ��֪Ϊ���з�Ƶ����

#### ��ֲ����
����ֲ��ͨ�ϼ�Ⲣ������IIC���ܺ�ϵͳ�������쳣��������ʱ��Ĳ��ԣ�ϵͳ��ͬ�����ܴ���½����޷�������Ŀ��ʵʱ�Ե�Ҫ�󣬹���ֹFreeRTOS��ֲ
### ͨѶ����

����ָ�����࣬��ѭ���͵ĺ������ܻ�������⣬�����Ծ���Ҫ�������5 DMA���͵�����

#### ����ͨѶ��ʱ�ж�

�������δ���ܵ��㲥֡������ͨѶ��ʱ��ֹͣ��⹦�ܣ��������ģʽ

#### ��������ʱ϶

�ڵ�һ��ʱ϶ǰ����һ������ʱ϶��������λ��ָ���ʱ϶

#### ���������ظ�

��ָ��֡ʱ϶���������ظ�������Э�鷢�������ظ�֡�����豸�������ģʽ����һ�ν��յ��㲥֡���ٴη��������ظ�

#### ������������λ��

##### ���ڵ�������

��python�Ա�һ�����ڵ�������

- �����ܵ���ÿ���ֽڱ���Ϊʮ�������ַ������洢��txt�ļ��У��������ޣ��������л�

```python
import tkinter as tk
from tkinter import ttk
import serial
import serial.tools.list_ports
import threading
import binascii

class SerialDebugAssistant:
    def __init__(self, root):
        self.root = root
        self.root.title("Serial Debug Assistant")

        # Serial Port Configuration
        self.serial_ports = ttk.Combobox(root, values=self.get_available_ports())
        self.serial_port_label = tk.Label(root, text="Serial Port:")
        self.baud_rate_label = tk.Label(root, text="Baud Rate:")
        self.baud_rate = ttk.Combobox(root, values=["9600", "115200"])
        self.connect_button = tk.Button(root, text="Connect", command=self.connect_serial)

        # Text Area for Displaying Data
        self.text_area = tk.Text(root, state="disabled", wrap="word", height=10, width=50)
        self.scrollbar = tk.Scrollbar(root, command=self.text_area.yview)
        self.text_area.config(yscrollcommand=self.scrollbar.set)

        # Entry for User Input
        self.input_entry = tk.Entry(root, width=40)
        self.send_button = tk.Button(root, text="Send", command=self.send_data)

        # Layout
        self.serial_port_label.grid(row=0, column=0, padx=5, pady=5)
        self.serial_ports.grid(row=0, column=1, padx=5, pady=5)
        self.baud_rate_label.grid(row=0, column=2, padx=5, pady=5)
        self.baud_rate.grid(row=0, column=3, padx=5, pady=5)
        self.connect_button.grid(row=0, column=4, padx=5, pady=5)

        self.text_area.grid(row=1, column=0, columnspan=5, padx=5, pady=5)
        self.scrollbar.grid(row=1, column=5, sticky="ns")

        self.input_entry.grid(row=2, column=0, columnspan=3, padx=5, pady=5)
        self.send_button.grid(row=2, column=3, columnspan=2, padx=5, pady=5)

        # Serial Port Object
        self.ser = None

    def get_available_ports(self):
        ports = [port.device for port in serial.tools.list_ports.comports()]
        return ports

    def connect_serial(self):
        port = self.serial_ports.get()
        baud_rate = int(self.baud_rate.get())

        try:
            self.ser = serial.Serial(port, baud_rate, timeout=1)
            self.text_area.config(state="normal")
            self.text_area.insert(tk.END, f"Connected to {port} at {baud_rate} baud\n")
            self.text_area.config(state="disabled")

            # Start a separate thread for reading from the serial port
            read_thread = threading.Thread(target=self.read_from_serial)
            read_thread.start()
        except serial.SerialException as e:
            self.text_area.config(state="normal")
            self.text_area.insert(tk.END, f"Error: {str(e)}\n")
            self.text_area.config(state="disabled")

    def read_from_serial(self):
        with open('received_data.txt', 'a') as file:
            while True:
                data = self.ser.readline()
                if data:
                    hex_data = ' '.join(format(byte, '02X') for byte in data)

                    # Check if 'A5' is in the data
                    if 'A5' in hex_data:
                        hex_data = hex_data.replace('A5', '\nA5')
                        
                    file.write(f"{hex_data}\n")
                    file.flush()  # ����ˢ�µ�����



    def send_data(self):
        data = self.input_entry.get()
        if self.ser and self.ser.is_open:
            self.ser.write(data.encode('utf-8'))
            self.text_area.config(state="normal")
            self.text_area.insert(tk.END, f"Sent: {data}\n")
            self.text_area.config(state="disabled")

if __name__ == "__main__":
    root = tk.Tk()
    app = SerialDebugAssistant(root)
    root.mainloop()
```

- ����ʱ���

##### ���ݽ�����ͼ

- ��txt�е����ݽ���������Ϊ��ѹ-ʱ������

```python
import matplotlib.pyplot as plt

# ��ȡ��־�ļ�
with open('received_data.txt', 'r') as file:
    lines = file.readlines()

# ��������
data = []
for line in lines:
    fields = line.split()
    if len(fields) >= 3 and fields[2] == '09':
        if len(fields) >= 5:
            hex_value = fields[3] + fields[4]
            decimal_value = int(hex_value, 16)
            data.append(decimal_value)

# �ضϵ���ӽ���ʮ�ı���
truncated_length = (len(data) // 10) * 10
data = data[:truncated_length]

# ��������ÿʮ������ȡһ��ƽ��ֵ
downsampled_data = [sum(data[i:i+10]) / 10 for i in range(0, len(data), 10)]

# ����ÿ�����ݵ��Ӧ��ʱ�䣨�Է���Ϊ��λ��
time_interval_minutes = 15 / 60  # ʱ����Ϊ1.5�룬ת��Ϊ����
time_points = [i * time_interval_minutes for i in range(len(downsampled_data))]

# ����ɢ��ͼ
plt.scatter(time_points, downsampled_data)
plt.xlabel('Time (minutes)')
plt.ylabel('Average Decimal Value (Downsampled)')
plt.title('Downsampled Scatter Plot of Decimal Values over Time')
plt.show()
```

�������еĳ�����ߣ������ݶ�������ѹС��3.7Vʱ��Ϊ�͵���������д������֡�����ָʾ��

##### ���Ϊwindowsƽ̨��exe�ļ�

��ƽ̨���У�������ִ�У����ڵ��������ubuntu�д��windows�Ͽ��õ�exe�ļ�

- �ڹ�����װ���°汾��wine8
[https://wiki.winehq.org/Ubuntu_zhcn](https://wiki.winehq.org/Ubuntu_zhcn)
```bash
# ��������python3.8
wine python-3.8.9-amd64.exe
```
- wine��װpip
��������pip��Ȼ��cd����ѹ��ɵ�pipĿ¼���߰�װpip

```bash
wine python.exe setup.py install
```

```bash
# ��ѯwine��python3��ַ
winepath -u "$(wine python -c 'import sys; print(sys.executable)')"
# ���wine��python3��ַ
~/.wine/dosdevices/c:/users/fdrc/AppData/Local/Programs/Python/Python38
```

```bash
# install maplotlib
wine ~/.wine/dosdevices/c:/users/fdrc/AppData/Local/Programs/Python/Python38/Scripts/pip.exe install matplotlib
# install pyinstaller
wine ~/.wine/dosdevices/c:/users/fdrc/AppData/Local/Programs/Python/Python38/Scripts/pip.exe install pyinstaller
# wine install serial
wine ~/.wine/dosdevices/c:/users/fdrc/AppData/Local/Programs/Python/Python38/Scripts/pip.exe install serial
wine ~/.wine/dosdevices/c:/users/fdrc/AppData/Local/Programs/Python/Python38/Scripts/pip.exe install pyserial
```

```bash
# -F means pack only one exe file
# -D means pack a dir
# --hidden-import= ѡ��Ҫ�����İ�
# --noconsole ȡ�����������д���
wine ~/.wine/dosdevices/c:/users/fdrc/AppData/Local/Programs/Python/Python38/Scripts/pyinstaller.exe -F --noconsole --hidden-import=matplotlib bat_analyze.py
```
- pack the serial_tool
```bash
wine ~/.wine/dosdevices/c:/users/fdrc/AppData/Local/Programs/Python/Python38/Scripts/pyinstaller.exe -F --noconsole --hidden-import=pyserial --hidden-import=serial serial_tool.py
```

Ϊ������ԣ��ֿ���һ����ֵ���������������ܵ��Ĺ㲥֡����������ӡ��ֵ��
�����߼��ɲο�֮ǰ�ĳ�����Ҫ�����ǽ�C����ΪPython
1. ���ܵ�����������֡������������
2. �����ж�ȡdata���ݣ��ɴ˻�����ֵ��

- ���ܲ������㲥֡
- ���ݹ㲥֡���ƶ�����ֵ��
- ������ֵ�����������
- ���������Խ���Ԫ��

```bash
#!/usr/bin/python3
# insert num 1 on the diagonal of the table which add a row not col
hex_num = 3
num_rows = num_cols = 3

table1 = [
    [0, 0, 0],
    [0, 0, 0]
]

flattened_list = [element for row in table1 for element in row]
for i in range(hex_num):
    flattened_list.insert(i * (hex_num + 1), 1)

table = [[flattened_list[i * num_cols + j] for j in range(num_cols)] for i in range(num_rows)]

print(table)
```

#### GATEWAY(ZSL420)-TWO NODES DEBUG

Modify a lot to the system, single board test has passed and all the function include data collect, configuration receive, data upload goes well. Now, it's time to test the nodes system.

The key is to test the detection ability between node to node. May all things goes right. First issue coming, gateway can only send, cannot receive the data. HHHHHH, it's not my problem this time after all. Got a little time to free. The gateway goes wrong....

Data sending goes right, but the receive goes wrong. For the last part of the system debug, is the two nodes connecting debug. connect two nodes together to see if there is something wrong.

- pin set time tick check ? 2023-11-03

For a accurate system tick, every pin check must be fine.Note the test files down. According to the test, the tick is very accurate. The protein problem may be the tick during the real detection.

- fix the pin_counter ? 2023-11-03

Another problem is that the pin_counter may be out of range every 40mins. One way to solve is modify the int16_t to int64_t, just expand the memory of the pin_counter. For the real node broad, the usart may modified.  
Unique ID: 37 32 46 C6; write the unique id onto the board to make the ID clear.

- fix the transmitter hub ? 2023-11-03

After fixing, the transmitter going well. Now, the gateway could be able to receive the right data. It's time to check the EXTI function.

- fix 0248 issue ? 2023-11-06

: 0000 0010 0100 1000 -> 0248  
: 0000 0010 0110 0000 -> 0260

slow down the system tick, now the period of the is too fast to debug, all the problem from the debug cannot be detected. So, now slow down the speed of the period.

A5 FF CC 29 89 00 00 05 19 00 37 32 48 55 0B 00 37 32 7F A2 02 00 37 32 39 66 05 00 37 32 46 C6 03 00 00 00 00 05 04 00 3A

���ܵ�ԭ��

1. �ⲿ�ߵ�ƽ��������
2. EXTI���������ű���ʼ��, the real reson is that the EXTI register is not deinit right after the trigger, to solve this problem, maybe I need clear the register of the EXTI by hand while the exti_deinit is clear all the exti register

һ��EXTI�ݴ����󣬳���ʱ��Լ�̶��������ڣ��ǹ̶�ʱ�����ǹ̶����ڣ��������������

�ƺ����ⲿ�ж����ú����ģʽ�����г�ͻ������ͬʱ��������Ϊ���ģʽ���ⲿ�ж�����ģʽ����ʹ���ⲿ�ж�����ģʽʱ��Ӧȥ��ʼ�����ţ�����ʼ������Ϊ����ģʽ������ʼ���ⲿ�жϣ���ʹ�����ģʽʱ��Ӧȥ��ʼ���ⲿ�жϣ�����ʼ������Ϊ���ģʽ�����øߵ�ƽ��

Can the gpio pin mode change anyway? When it change the output mode and the input mode, is there a deinit?

1. without deinit
2. with deinit

After a lots of codes, the function goes right? The receive data is 0260, kinda weird? I don't even know what is going on. How did that make? While left the pin hanging, the value will vary from sevral values. At least the system pass the 2460 problem. Next is speed up the frequency of the system to test the limit of the system speed.

�����Ľڵ��������⣬���ܵ����ж���Ϣ���ͺ�һ�����ڡ�  
���������ݲɼ�ǰ�ƣ�������Ϊ�ɼ���һ�������ݣ����Ȳɼ���һ�������ݣ��ٽ����������ã��Դ���������ʱ���ӳ�һ�����ڣ�����ͨ����Ŀǰ50ms��ͨ�����ѻ�����������Ҫ�󡣺������ܻ��������Ż�.

One problem is that the stability of the system, when the nodes and the pins increase, still the system goes well? For now the two nodes and two pins excute well.

- delete the value on the diagonal ? 2023-11-07

To be honest, although this is an easy operation, I still make something wrong, the main reason is that I didn't have a clear understanding for the real system by the way the code of the ?upper monitor, the code of the upper monitor is too naive which make that I can't even change any of the upload frame...

1. Delete one value in the array and push forward the rest of the value behind the certain value.
2. Modify the length of the pack and the upload frame.

Up to now, the TWO NODES SYSTEM DEBUG is over, and the result is OK.

No, diagonal figures cannot be deleted by this easy way. I finally use the peicewise function to solve the data location varies from the stage of the function. To be honest, I don't know a better way to solve this but peicewise function. After all, the key to the program is the math....... cause the program'  
key is to process data, and the key of the data is mathmatic. Finally, I think all things done.

- fix the first order wrong ? 2023-11-07

Forget to test the first one board, now the first one board got something proble, I guess the counter - 1wrong. Especially those values that minus 1. Almost all the problem is caused by the minus value. I think if there is some function to detect the minus values automatically?

It seems like the first edition of this project can be released. All the function has be done. Next, I think is deeper system develop, like the power saving mode, the adc collect, power saving mode is the most crazy mode, cause it will shut things down while making all the debug difficult.

Finish all tha code standarlization, now the code looks more clear and beautiful.

#### DEBUG-GATEWAY(ZSL420)-NODE

The gateway was changed to the zsl420 which need new a new system debug. The purpose is to check if the code is running well with 5 nodes system.

The expriment has encome some problems, but the result is fine, the 5 nodes broadcast frame is parsed well, all the data are parsed and stored in the node memory. The second point is that the slotvolume is defined by the bit not the byte, in the tow byte memory, first 10 bit is line number, and the last 6 bits is slot number which makes me confusing for a while.

- parse the 5 nodes broadcast frame ? 2023-10-31
- data collect test for 5 nodes system ? 2023-11-01
- read the device ID in the chip ? 2023-10-31UDNIQUE ID is stored in the 0x1FFF7A10 address, while using, read from this address is OK.

Before the system test, the code may be supposed to modified a liitle. Fonts problem goes a little, the most problem is that the problem is not recongize a lot. ?Change the fonts may modify the effecient of the coding. Code does not meant be everything, all the code and debug server for the life the work or to satisfying self.

Some function may change the form to the pointer which now is using the array dot. Another problem for the developing is the hardware, there is only one zsl420 so I can't make all nodes connected. To make the synch exprimental, I wonder if there is a way to let the serial debugger to transfer a data at the same time? At least using two of them would help me to debug the system. Oh, here comes another problem, the USB TTL device is not enough. It seems like there are multiple problems.

Given one solution to parrallling the TX and RX.

Here begin the debug, the nodes can only receive the first broadcast frame, the following frame cannot pass the length check? Why? There is align error, the first frame length matches, but the second one failed, I wondered which part gets wrong, which parts? I mean there maybe the gateway's wrong, I should check whether the usart works well or not.

Back to the edition yesterday, everything seems works well, then change the pin period to the 1M, problem comes again. It seems like the period cannot be too short? ?What's wrong? Another thing is that as long as the pin peirod greater than the node period, the things goes right.

- ==ISSUE==: Pin period cannot set less than the node_period ? 2023-11-01

possible reason:

1. After enter into the collect period, the collect function ruins the whole code period.
2. It's the memory copy funcion error, the memcpy has copied a minus address into the memory which smash all the data before that address.

Even though I have found where the problem is, I am so tired to fix that problem. Maybe I should do some other things to relax. Fix this iusse then debug then comes another issue, bug cannot be fixed all, there is always bug.

What about the variable name? pin period, slot period, node period, if it needs a instruction for those variables.

- ==ISSUE== slot data collect gets something wrong ? 2023-11-01

When PinCounter equal to PinNumBefore(i) means the current slot stamp. Only the pin counter synch, and how to know the slot counter? The relationship between the pin counter and the slot counter is the pin num. After counting the whole pin nums, then come to the next slot counter. Make a slot counter index? When to make the index? Every time receive the broadcast frame? The direct method is to make a slot counter index, everytime collect time then check if it match the index and store the pin num into????

Every after the pin time, tranfer the pin data buffer to the node data buffer!!!! ALL THE DESCRIPTION ABOVE IS WROOONG

For keil dpi problem, keil always looks blur, now change the keil's dpi to fit the windows 10, problem done.

- fix the dynamic node period buffer length ? 2023-11-01

The length of the node period buffer is according to the slot num and the slot pins. So it's not a static array, but in the c compiler the array should define before the compile.Tired,tired,tired,tired.  
Using the memory and the pointer method to control the byte length of the array. The pointer of the structure could add more bit when data is more than the one byte automatically.

- EXTI wrong ? 2023-11-02

Now, the EXTI cannot get the corresponding interrupt value.  
The reason is that the priority of the interrupt, the EXTI interrupt should be the first, next is USART, then is Timer. That will make everything's OK.

- fix data register transfer flow ? 2023-11-02

After fix the EXTI, the pindata -> slotdata -> node data goes wrong, the whole flow cannot goes approriate. Take a lot time to fix this flow.

- fix the data send frame ? 2023-11-02

The protocol changed, so the frame of the data send should fix agian. All the data protocol define has been done.

#### PROTOCOL MODIFY

In the day befoe, I happen to find that the memcpy function cannot store two byte array into one uint16_t array. ?Given a source file and the header file, which include the protocal define and a `#pragma pack(1)` code which is meant to allocate one byte location for the uint16_t data to solve the problem above which is wating to check

- check the `#pragma pack(1)` working? ? 2023-10-30It seems work well, the node congnize two uint8_t data into one uint16_t data.
- fix the pin data collect issue ? 2023-10-30Done, the key reason of the issue is defining the array length during the function by the size of function. The arrayLength should predefine instead of defining in the function calling.
- fix the order of the uint16_t data ? 2023-10-30When receiving the broadcast from the gateway, especially the TotalNum(uint16_t), the receiving form is two uint8_t data. when receiving the 00 01, it will stored in form of 0x0100, I don't know how to describe this issue. NO NEED to fix at least for now
- XOR checksum ? 2023-10-30When pack the data, there should be a XOR checksum for a accurate data check in order to reduce down the effect of losing data during the process of sending data. While designing the function of the code, there is also should be a check num, which is the return number in the function, return flag, return 0 means failure, return 1 means success. Call the CheckOut function to
- modify the sequence number ? 2023-10-30Sequence number will add one every period finished. Add a seqnum++.  
    Another change is to use the ZSL420 for gateway instead of GLCOM-NET. ?In that case the debug system changed all, at least there is no more UDP server no more ethernet no more GLCOM NET. By the way, there is only two zsl420 telecom board and make it more difficult to debug the whole system.

While debugging, there is a problem that confusing me in a long time which is how to check my modify step is correct or make sense? It's very difficult to define what is correc and what is incorrect, in general, this is judge by the sense instead of principle. In that case, inverse the oder of the uint16_t data independent. But most importantly, I didn't use the total slot num and total pin nums. In my code, the start of the period is trigger by the broadcast while the stop of the period is counted by the timer.

#### BROADCAST FRAME

- ���ͣ�����ʱ�����ṹ��serializationת����ÿ��byte array��Ȼ��ÿ��byte arrayͨ�����ڷ���
- ���ܣ�����ʱ�������ܵ�byte array deseriallization��Ȼ����ÿ��array lenthȥ��ȡ��Ӧ���ݣ���ȡ�����ݷ����Ӧ�Ľṹ����

### ʱ��ͬ��

Ŀ�ģ�ʵ��ÿ�δӴ��ڽ��յ���������Ϊ�㲥֡�����ݼ�������ʱ��ͬ��

�ֲ�����

- ʵ�ֶ�ʱ��ͬ������ ? 2023-10-17
- ʵ�ִ����жϴ�����ʱ��ͬ�� ? 2023-10-17
- ʵ�ֹ㲥֡�����豸ʱ϶ ? 2023-10-23
- �㲥֡�������� ? 2023-10-19
- �����������洢������ ? 2023-10-19

1. printf ��һ���ǳ������ָ���ʵʱ��Ҫ��ϸߵ�ϵͳ�У�Ӧ����ʹ��printf

- ��ģʽ�л���������ģʽ���ж� ? 2023-10-19

����Ƶ�GPIO��ģʽ�л���ʱ��Ҫȷ���ڵ���GPIOǰ��GPIO�Ѿ�deinit������init������ĳ�ʼ����ȥ��ʼ��ͬ����Ҫ

#### UPLOAD PERIOD

slot_order indicates the order of the mode switch  
When all the last of the slot_order finished, here come to the data_send_time. In this time, all the nodes send the data one by one, with another order? It seems like that the slot_order can be used again, deinit this order and make the nodes upload the data by this slot_order by the mean time the data_upload_time should be enough for all the upload time including data camping time and the time on air.

Meanwhile, the the whole period is divided into two seperate period, collect period and the upload period, in the collect period nodes are arranged to be the master mode and the slave mode one by one, and in the upload period, nodes need to upload the data in the order like before, but more accurately and precisely with a lone time.

Upload at the beginning of the node time.  
DMA! Waiting for the DMA function? Develop a functino to upload the data by the DMA form, second, waiting the function

#### COLLECT PERIOD

During the collect_period, the nodes needs to collect the EXTI data in every node time and store that data into a period_data which will be erased when a whole period done.

So, in each node time the node collect the EXTI data into the period_data_buffer. When the node time runing off, memcpy the period_data_buffer into the period_data. When the whole period done, nodes send the period_data to the gateway in slot_order every a certain time.

To be honest, the variable define and arrange is neccassary and diffucult. I can't define tidy?  
How to know that the whole collection period running out? The slot_num indicates that how many slots in the current period. when the slot_num runs out means the collection period close  
no, it seems like that the upload should combine into the collect_period. While the whole data is collected, the node should combine the data together, and then send out.

Meanwhile, the upload fucntion should use the DMA which is more efficient.DMA example code gives a very clear function how to get the DMA done. While using the DMA, user should config the DMA first, and config the nvic next, then use the DMA interrupt to read the FLAG of the period of the DMA to check if the DMA transfer task is done. When DMA transfer off, checks if the full transfer flag (FTF) has been set for DMA1 channel 7, indicating that a DMA transfer has completed. By using this FLAG user can easily know when the DMA done.

So, for the code I develop, the DMA transfer seems to enable automatically, the trigger logical should be define. No use for logical just automatically send, every time enter into the master mode, enable the DMA, enter the slave mode and disable the DMA which means do not send the data out.

#### MODIFY THE PIN_MASTER_LOOP

Change the gpio_set loop into the EXTI loop.

For example

```
gpio_set0 -> 1000 -> gpio_reset0 -> exti_deinit -> 1000 -> gpio_set1 -> 1100 -> gpio_reset1 -> exti_deinit -> gpio_set2 ->0010 -> gpio_reset2 -> exti_deinit
```

In case without the exti_deinit

```
gpio_set0 -> 1000 -> gpio_reset0 -> 1000 -> gpio_set1 -> 1100
```

In the pins loop mode, before ervry borad's pin set high, the binary_buffer should init. The key to this problem is that the pin may not belong to the board itself. The board A pin should synch with the board B pin.  
There is no master node, only the master pin, but the data send should between first pin of the certain node. No, keep the master node, but add a now loop called the master pin.

Set a example for that is node A and the node B, every node has 2 pins. For the first time, make this situation realize for a better try. Even in the 2 nodes situation, the pins_master_loop is so difficult, every tick the pin toggle the mode, and give the bang to the next pin. I have deleted every other out data to leave the PE0 and the PE1 enable while use the for loop to realize that every tick init the gpio, set the gpio, reset the gpio, collect the data into the IR_data_buffer then the period_bin_data_buffer, then intial the IR_data_buffer to store the data in the next period. In theory, this would work well, and collect the data in the period_bin_data_buffer which will send out at the beggining of the master mode enter.

Alongside the pin_master_loop, there must be some node_master_loop, after one node's pins loop finish, here comes another node to begin the pin_master_loop. Whether the node loop and the pin loop, it can be done by the timer while using a counter. Use a global period_counter to count the timeline.

#### USE THE PERIOD_COUNTER COUNT THE PIN SWITCH

For example,  
total_line = 4 = sum(slotline[i]), slot_line[0] = 2, slot_line[1] = 2  
The period_counter is the timeline, so when a node's pin to start? When it comes it's pin_start_time, ?it will start the master mode.  
pin_start_time = sum(slotline[i-1])  
from the sum(slotline[i - 1]) to sum(slotline[i - 1]) + slotline[i]  
Every slot time contains multiple pin time, in this example the whole system contains 2 slot, and each slot contains 2pins. Using the pin_master_set(GPIOE, EPINS[period_counter - pin_sum[i-1]]),

Almost done, only use one counter or timeline, whether the node loop and the pin loop depend on this period_counter produced by the timer1. In this pattern, there is no more node_loop, only the pin_loop and the node timetick is counted by the pin num. Every period finished the period_counter will refresh meanwhile the period_counter is the pin_counter cause every counter the pin toggled.

Something done, but not done. ?Only this way can the programe go on. There are pin_counter and the slot_counter, pin_counter++ every one timer interrupt, the slot_counter is extracted by the broradcast frame. Check if it is in the slot, then set, if it's not the right slot, there will be no set. No matter what the slot is, the IR_bin_data_buffer collect in every pin_period( pin_period, slot_period, node_period )

Already finished the pin data collect, then debug.  
All bugs are fixed, waiting for the download test......  
Recongnize the software frame, reduce the extra source file. Compiling, build, almost finish every function. The ideal compiling result is that receive the broadcast frame then start the node period, configure the node information by the broadcast frame, start the pin master mode one by one according to the broadcast frame. After all the slot done, all the nodes stop until the next boradcast frame

Waiting for the last expriment, checking the function. The process of the debug is a little confusing, which you can't know what you will done for the system or the program. Everything was so different when you change the code structure. Whenever change the code a little, there maybe a lot of buggggs, fixed by hand first, then debug on the board, continue to fix the bug. After all bug fixed, here comes the function check, the function will be broken in some time. Then its time to fix the function, this is the most difficult ?cause the function may have multiple situations.

In the first time, send the broadcast frame, the node enter the master mode successfully. But when resending the broadcast frame the node didn't restart the master mode. It may stuck into a certain loop. As long as there is a delay_1ms, the timer will be stuck and the loop shut. The reason is unknown. Reason is that the register is out of range, it should be restriced by something. OH, it is my IF judge command wrong, it can't be the a> x >b where the correct answer is a>x && x>b

#### DATA_SEND_FRAME

According to the communication protocal, whether data_send_frame or the data_send_frame must designed in data structure. In case that the communication protocal has changed since last meeting finished. The data_receive_frame and data_send_frame need to be reshape according to the communication protocal.

Change the data structure may cause a lot of problems, backup when needed.  
Changed the data structure and it's link variable, then compiling to check.  
Something problem about my code editor and the backup formula, I have ?mess everything! All right, it's just a function error, repair that bug just need a little time. Done.

Data_send_frame structure need modify. Got a problem that uint8_t and the uint16_t data process. My receive code can't receive the uint16_t data but only uint8_t data. Now I'm developing the uin16_t data send function, according to the code before, I have made a DMA to send the data from the memory to the USART. The structure need to transform into the buffer or the byte array before using the dma send.

Send_data_buffer before initial the period_counter during the memcpy the pin_bin_data_buffer, the counter was added 1 already.

All things done. All correct while using some tricks. Tricks may cause some potential problem in the days, but for now only the results mattter.

### ϵͳ����

#### GLOBAL VARIABLE CONFIG

All the variables, typedef, macro define should be put together into a files  
I am learning how to put all the variables together which will help to make the whole code more clear.  
Up to now, I have put the defines intot he variable_config.c and make a variable_config.h put the macro defines and the extern define. I don't know whether this right or not.  
Almost done, all the data is configured well. From now on, the variable can only be changed into this globle_config map, oh no, I think the globle map matches better.  
Already rewrite the global config, debug indicates that the data send function has something wrong  
Here comes another problem, when slave changed from the master will make the EXTI diaable, the possible reason is that the GPIO configure. Waiting for the check.  
Iuuse done. The reason is that the master mode set a while(1), change to the variable mode then make the problem done.

There is something problem which every global variable is not desbribed well. Many variables are just but together for no logical name and category. In which logic to arrange the variable?

#### SOFTWARE & HARDWARE SYSTEM DEBUG

Now, the first edition of the software is finished and the solo board debugging pass.  
For now, the software has realized those function which are receiving the command, configure the rank by the command, collect the EXTI data, send and packing the EXTI data. OK, that's all, the DUBBUG is to confirm all the function is work well on the two board system.

The first board is 2001, the second board is 2002, the deafault slot order is -1, the deafault mode_falg is -1 (In my code, I have design the -1 to the deafault mode while deafault mode means to do nothing)

Find another wrong while preparing the system debug. The main reason is that for the one board the borad coporate data cannot be design, but in the system debug, the data contains every pins in every node which makes the data more complex. So here comes the problem is that I used the node time to switch, but the destination is the pin time. Switch the mode between the pin from the EXTI mode and the collect mode. Even when in the master mode, and the first pin is the giving high level (master mode), the other pins in the same board should in the slave mode coporate with other borad's pins. Master_gpio_loop should be modified.

#### OPTIMIZE CODE READABILITY BY THE LAW

Before step into another stage of the program, the readability of the code should be optimize or modify first. The main part is to add the comments for those sub function, meanwhile rearrange the structure of the code.

All the fuction are commented in the law of the Doxygen while left the structure to comment and make it clear. All the variable and the function are comments done. Even though I can't promise that every variable and the function will be the fine to run, at least for now codes runs well

The next thing is to modify the format by the law in the company which is a little confusing. The key is the format and how to follow the fomula of the company.

#### ϵͳ����ͼ

```mermaid
flowchart  
A[System Start&Deafault Mode] --> B[Receive BroadcastFrame] --> C[Parse BroadcastFrame] --> D{Header match}  
D --> |Match Success|E[DeviceID matching]  
D --> |Match Fail|F[PowerSavingMode]  
E --> G[Config Device] --> H[Timer&Counter Synch] --> I[Check PeriodCounter]  
I --> |Match Success|J[NodeDataUpload]  
I --> |Match Fail|K[PinSlaveMode&CollectData] --> I  
J --> L[PinMasterMode] --> K
```
#### SYSTEM DEBUG

For now, the single node has debuged pass, functions including receive data, config device, time synch, data collect, data send are all pass. After that, there should be the system debug to check the all system work well.

The whole system contains devices including UDP server, gateway, several nodes. First, establish a udp server on PC by the upd server tool. Second, change the gateway mode to the udp client mode and connect the gateway with PC by the ethernet wire. Third, connect the GD32F470 with the ZSL420 by the way of USART, and initial these two borads system. That's all. But anyway, I just found there is seems like a problem? Or something not done?

Data collect does have something wrong. No matter the hardware link is, the pin data is 0x02 where the pin data should changed according to the hardware link.

### �͹��Ŀ���

#### GD32 POWER SAVING MODE

According to the talk last week, I have a better understanding for this system's power saving mode. Design the PowerSavingMode using the Duty cycle which is the main body of the power saving but the real parameter of the duty cycle is complex. To determine the parameter of the duty cycle need balance the power and ?performance of the system.

Ah, enter the deepsleep and the wakeup function should use the timer to count, so how to use the timer to count??? If the timer can be use to produce the interrupt and the count at the same time?

- GD32 SleepyEnterStandbyMode ? 2023-11-13

![](https://cdn.nlark.com/yuque/__mermaid_v3/1b8dda25b42ebc0c856f16b309fd20b5.svg)

The result presents that the current of the normal is 90mA while the standby mode is 80mA. Emmmmmm, something wrong. Rewrite a blank project just including standby function to test the blank system current

No matter how, the PowerSavingMode do save some power, the relevant value is 10mA, at least for 10mA. The code above is all about the power saving on GD32.

- Blank system design & Current test ? 2023-11-13

The result didn't change a lot makes the blank system no meaning.

- Shut down extern power customer ? 2023-12-01

While the GD32 is not the only power customer in this system but the ZSL420 and those boost circuit. Those should be shut in the power saving mode.

The solution is set the pin PORT CTRL low when designing the real system. This will cause all the power extern shut down. But this need the real system not for now.\

#### SX1268 POWER SAVING MODE

The most important part is on the LoRa board system, the current design is that shut down the sx1268 all the time but during the data upload time, in that way, for example the wireless function is on for at most one pin period during the whole node period. This will save more power when the pin nums goes more.

- Finish the wireless power saving mode

In another, the communication of wireless function will change from the usart to spi, control body will change from the ZSL420 to the SX1268. Use the spi periphral to directly control LoRa communication chip SX1268 will increase the performance and decrease the power losing.

- Change the communicaiton soc from ZSL420 to SX1268

### ADC����

#### RESISTANCE MEASURE

ADC was config well since last time, Now I should take back the ADC to read data first to check if it works well for now; Then configure how to store these data and when to read these data? Repair done, the data collected is stored into the array. The ADC data collect logical is so different from the EXTI, the adc line form is must pin to pin while no other connectiong form. So, the ADC collect can stary for all if the Lline greater than the Zline(what about if the Zline greater than the Lline?)

- Read data and Store the data according to the protocal ? 2023-12-08

1. Data Type. According to the disscussion, there is 3 wrong type, short circuit, open circuit, misconnection. So the data should include all the data, otherwise, it's a mirror for the Lline. This is data context.
2. Collect time. Before data collect, the ADC value should be in a steady status �ڴ����ʱ��ɼ�����ζ�ŵ�ADC��δ��ȫ�ȶ�ʱ�ͽ���ADC�ɼ����⽫���²ɼ�����������ȫ������ô����ADC�ɼ��ڶ�Ӧ������������ִ���أ�������ж��н��У���α�֤ADC���Ĳɼ�ʱ�䣿�������ѭ����ִ�У���α�֤�ڶ�Ӧ��ʱ϶ִ�У�
3. Data process. The data collected from the ADC may varies from a range of values. The data must be proceed before upload. �����˲�������һ���ѵ㣬���ʵ��˲�����õ���ȷ����ֵ�����ܵ��˲������У���ֵ�˲���ƽ��ֵ�˲��������˲����ڱ���ϵͳ�У��������Ե�ģ̬�ֲ����ʲ��������˲�Ӧ�û��бȽϺõı��֡������⽫��һ�������ڵ������˲�

�ٴ�����迹��⣬�ѵ�����֮ǰ�ĳ�������в�û�п��ǵ��迹��⣬��͵���������迹����ʱ��Ҫ���¸���ԭ�еĺ����ͱ������ơ�

���ڣ��Ѿ�����ԭ�е�ͨ�ϼ���߼��ظ������һ����ѹ�������飬���Ҹõ�ѹ��⽫��ͨ�ϼ�Ⲣ�У�����ܻᵼ��Ǳ�ڵ����⡣�����Ѿ�ͨ������ʵ���������Ƿ��ܹ���ȡ����ѹֵ����ѹֵ�Ƿ���ȷ���˲��Ƿ�ɹ�����Щ������Ҫ��ʵ��ϵͳ�Ͻ��в��Բ�����.

ADC֮ǰ�Ѿ�д��һ���⣬ʵ�ֵ�ADC���ݲɼ����Լ���ŵĳ����ܣ�����ֲ�����ڵ���ʽ�汾������һЩ����

��ֲ��ԭ���Ǳ�֤ԭ�е�L���û�����������£�����������

ADC�ɼ���ʱ���᣿  
ADCʱ���������ͨ�ϼ�⣬������Pin period ��������������

��ͬ��ͨ�ϼ�⣬ADC���ŵ�����ģʽ�ֱ�Ϊģ������͵͵�ƽ���

#### ��ʱ���

��ͬ���ⲿ�жϣ�ADCÿһ�����ŵ����ݶ���Ҫ��ѭ���м�⣬�������Ӧ��������

���Aһ�����������ţ�B���������ţ���A1����ʱ��A��Ҫ���A2��A3, B��Ҫ���B1��B2�� Ҳ����˵���迹��⸲����ͨ�ϼ�⹦�ܣ�����������ܵĻ����������ADC���ݵĲɼ�

�ڲ��ı����е��������ڵ�����£�ÿ������������40ms��Ҫ��40ms������������������ŵ�ADC���ݣ��ȽϺõķ����Ƕ��������Ž�����ѭ��⣬�������ݴ���ADCbuffer�С�֮�����ϱ�����ʱ�����Ƚ������˲���Ȼ�����sendbuffer�У�Ȼ��ͨ��lora�����ϱ�

Ŀ֮�������Ѿ�ȫ�������ģ������߼������ݲɼ���Ȼ�������ݴ���ϴ�

���ݴ���ϴ��Ѿ���ɣ���������ADC������֤����Ҫ�޸Ĺ㲥֡���迹����������Ŀǰ�㲥֡��û���迹���������������޷�������֤

���������Ʋ��ӵ������������Ҫ�����ؽ��и��ģ�����һ���迹������������������Ӵ������ù��ܣ�ͨ�����ڸ������صĹ㲥֡��ʵ��ͨ�ϼ����迹���������

#### ��������֡����Ϊ��ֵ

�迹��ֵֻ������ 0 120 �����

���Բ��÷ֶκ�����ADC��ֵ��������ӳ�䣬�ص��Ǳ߽�ֵ�Ķ��壬120��0��ʵ�ʵ�·��ADC���ֶȲ���

�����򵥲��ԣ�0 �� 120 �ٽ�ֵѡ��Ϊ0x810

���еļ�����Ϊ120ŷķ�����ⲿ������2280ŷķ�����¶�·���е����ADCֵ�ǳ��ӽ����ټ���ADC�������ⲿӰ�죬��ADCֵ�������¸������������޷�����120��0

��������Ƕ̽�1080���裬�������ⲿ�����Ϊ1200,��������Ϊ120��������120��0 ��ADCֵ���350����λ�����ױ����֣��Ҿ��ȸ���

|**��������**|**����ԭ��**| **�������** |
|---|---|---|
|�̽Ӻ�������||�麸�ˣ����º����޸����|
|A��78���ݴ�λ||����һ���˲�|
|78��FF���ݾ��Ȳ�|||

### I2C����

#### IIC TO READ PERIPHERAL

IIC��һ��ͨ�����ߣ��ص��ǵʹ������豸ͨ��

Initialize IIC first. The problem is that how to check if the IIC works? ��ʵ�ʵİ����յ��󣬽���ʵ�ʵ�IIC���豸�������ݶ�д������֮ǰ��������г��ԡ�

��ȡIIC ����Ĵ�����16bit����  
�ѵ����ڣ�IIC�������������Ǵ����uint8_t �����У���Ŀ��ռ��ǽṹ�壬��Ҫ���ַ�����ȡλ���ݲ���Ž��ṹ��λ��

#### xl9555 INT��-�ⲿ�жϿ���

��input���ŷ����仯ʱ��INT���Żᷢ��һ���жϣ����жϻ����������ŷ����仯ʱ�ٴη���

```c
eeprom_buffer_write_interrupt(I2cFaControlSet1, XL9555_config_reg_cmd_H, 1);
eeprom_buffer_write_interrupt(I2cFaControlSet1, XL9555_output_reg_cmd_H, 1);
```

������֤�����ģʽ���Ʒ�������  
���ȣ�����config�Ĵ��������ö�Ӧ������Ϊ���ģʽ0  
Ȼ�󣬿���output�Ĵ��������ö�Ӧ������Ϊ�ͻ��߸�

```c
eeprom_buffer_write_interrupt(I2cConfRegisterL, XL9555_config_reg_cmd_L, 1);
eeprom_buffer_write_interrupt(I2cConfRegisterH, XL9555_config_reg_cmd_H, 1);

eeprom_buffer_read_interrupt(i2c_buffer_read, XL9555_input_reg_cmd_L, 2);
```

������֤����������������  
���ȣ�config�Ĵ�������������ģʽ  
Ȼ�󣬶�����

���ϣ����ۿ���input����output�Ĵ������ڿ���֮ǰ����Ҫ��config�Ĵ������в�������Ȼ�ͻᵼ��IIC���߿���ʧ��

# ����

## ��Ӳ����

- �ж�����ӳ�� ? 2023-11-20

ʵ�ʰ����ϵ��ж����ŷ�оƬ������˳����Ҫ����ӳ������˳���Եõ�������ĵ��Է������������ֻ�漰��main�жϣ���ֻ����PB���ţ�

1. ���ж�����PE����ΪPB
2. ����PCBA����ӳ��PB��ϵ

������ɺ���Ҫ���¼�鹦������

- �޸�����DMA���� ? 2023-11-22

֮ǰ������ʹ�õ���USART2�������Ǵ����жϽ������ݣ�DMA�������ݣ�����������ͬʱ��������

���ڸ�ΪUSART5,�����жϹ�����������DMA���͹����쳣  
��ΪUSART2, ����ʹ��DMA�������������ô���ֱ�ӷ�����û�����⣬��ʱ����USART DMA

- �����߼��ɸ߸�Ϊ�ͣ����޸������߼� ? 2023-11-21

�������߼���·��ת�˵�ƽ���ã�ʵ�ʵ�·�д����߼��ɸ߸�Ϊ�˵ͣ���˳����е����д����߼���������Ҫ���±�д��������� masterset loop��˳��Ҳ��Ҫ���ġ�

��·�߼��ص����ߵ�ƽ�����󣬹���һ��������������λ������

## ��λ��-��λ������

- ����ʱ϶���� ? 2023-11-22

A11��B1������ʱ϶���棬��������ǰ�ƣ���ӵȴ������������ǰ�Ƶ�����

- �޸����һ�������������ݴ洢ʧ�ܵ����� ? 2023-11-23

���ݴ��ʱ�����ݳ��ȱ���1,�������һ�����ڵ�����ȱʧ������������ݳ��ȣ������Ӵ�������ڳ��ȡ�FIXED  
����Ǳ�ڵ����⣬�޷���Ԥ�ϣ�Ŀǰ�ĺ����Ƿ����δ���������⣬��Щ����δ֪�ģ��᲻��������⣬���ֵ�������ʲô���ģ�����Ԥ�ϣ�ԭ����û����Ч�ķ����ֶΣ��������ʵ��ϵͳ������̫��

����Ƕ��ʽ���������ԣ������Ӳ����ǿ����ԣ������linux�������ڴ���Ŀ��������£��и���Ч�ķ����ֶΣ�����δ֪����Ŀ����Ը�С��

- �����ؿ��� ? 2023-11-23

��ʵ�����⣬���ص���Ϊ�µ�GD32 + sx1278�������Ҫ���¿������س����µ����س�����GD32�����У�ͨ��USART5��SX1278����ͨѶ��SX1278��ΪLORAʹ��͸�����ܡ�

���ؽ������¼�������

1. ����5���͹㲥֡
2. ����5��������֡����͸��������2

�����еĿ�����־����¼��ͬһ�������ĵ��У���̫����ʶ�𣬼�¼�������Ծ����������ڣ�����һ���ܸ���ֻ��������