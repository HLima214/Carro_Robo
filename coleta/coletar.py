import serial, time, sys, os

PORTA = "COM4"          # troque pela porta do seu ESP32
DURACAO_S = 15          # duração de cada amostra
classe = sys.argv[1]    # ex: python coletar.py aproximando

os.makedirs("dataset", exist_ok=True)
n = len([f for f in os.listdir("dataset") if f.startswith(classe)]) + 1
arquivo = f"dataset/{classe}.{n:02d}.csv"

ser = serial.Serial(PORTA, 115200, timeout=1)
time.sleep(2)
ser.reset_input_buffer()

print(f"Coletando '{classe}' por {DURACAO_S}s... comece o movimento!")
inicio = time.time()
with open(arquivo, "w") as f:
    f.write("timestamp,dist\n")
    while time.time() - inicio < DURACAO_S:
        linha = ser.readline().decode(errors="ignore").strip()
        try:
            valor = float(linha)
        except ValueError:
            continue
        ms = int((time.time() - inicio) * 1000)
        f.write(f"{ms},{valor}\n")
print("Salvo em", arquivo)