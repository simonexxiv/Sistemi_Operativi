# Sistemi Operativi - Prove d'Esame (UNINA)

Questo repository contiene la raccolta delle prove d'esame svolte per il corso di **Sistemi Operativi** presso l'**Università degli Studi di Napoli Federico II**. 

Le prove sono state ripulite da tutti i file compilati (`.o` ed eseguibili) per consentire una compilazione pulita da zero tramite i rispettivi `Makefile`.

---

## 📂 Struttura del Repository

Ogni prova d'esame si trova all'interno della cartella `Prove_Esame/` suddivisa per data dell'appello. La struttura interna di ciascuna cartella è organizzata come segue:

```text
Prove_Esame/[Data_Esame]/
├── scheletri/        # File sorgente di partenza forniti dal docente (scheletro)
├── risoluzione/     # Svolgimento personale dell'esame (soluzione)
└── traccia.txt/pdf   # Testo dell'esame ufficiale
```

---

## 🛠️ Come Compilare ed Eseguire

Tutti i programmi sono pronti per essere compilati ed eseguiti. Per provarne uno:

1. Apri il terminale ed entra nella directory della traccia e dello svolgimento desiderato, ad esempio:
   ```bash
   cd "Prove_Esame/Luglio_2023/risoluzione"
   ```
2. Compila il progetto utilizzando il `Makefile` presente:
   ```bash
   make
   ```
3. Avvia il programma eseguibile generato (es. `start`, `server`, `client`, ecc.):
   ```bash
   ./start
   ```
4. Per ripulire la cartella d'esame dai file generati:
   ```bash
   make clean  # (se supportato dal Makefile) oppure rimuovi i file .o e gli eseguibili manualmente
   ```

---

## 🏷️ Suddivisione per Categoria di Esercizio

Le prove d'esame coprono diverse tipologie di costrutti per la concorrenza e la comunicazione tra processi (IPC):

### 🔄 Produttori / Consumatori
* *2021-05-17*, *2021-12-13 2*, *2022-05-26*, *2022-07-08*, *2022-12-16 A*, *2022-12-16 B*, *2023-02-28*, *2023-04-18*, *2023-06-16*, *2023-09-28*, *2023-10-26*, *2023-11-30*, *2023-12-19 B*, *2023-12-19 C*, *2024-01--24 1*, *2024-02-21 1*, *2024-03-18*, *2024-12-19 A*, *2024-12-19 B*, *2024-12-19 E*, *2025-01-14 1*, *2025-02-14*, *2025-03-05*, *2025-05-05*, *Compito_Esercitazione*

### 📖 Lettori / Scrittori
* *2020-12-21 10*, *2021-12-13 2*, *2022-07-08*, *2022-12-16 C*, *2023-02-28*, *2023-03-14*, *2023-12-19 A*, *2024-01--24 1*, *2024-01-24 2*, *2024-12-19 C*, *2024-12-19 D*, *2025-02-14*

### 🧵 Threads (POSIX Pthreads)
* *2021-05-17*, *2022-07-08*, *2022-12-16 C*, *2023-01-31*, *2023-02-28*, *2023-07-25*, *2023-09-28*, *2023-10-26*, *2023-11-30*, *2023-12-19 A*, *2023-12-19 B*, *2024-03-18*, *2024-12-19 B*, *2024-12-19 C*, *2025-01-14 2*, *2025-03-05*, *2025-05-05*

### ✉️ Code di Messaggi (Message Queues System V / POSIX)
* *2020-12-21 10*, *2021-04-19*, *2021-12-13 1*, *2022-07-08*, *2022-12-16 B*, *2022-12-16 C*, *2023-01-31*, *2023-02-28*, *2023-03-14*, *2023-06-16*, *2023-12-19 A*, *2024-01--24 1*, *2024-01-24 2*, *2024-03-18*, *2024-12-19 C*, *2024-12-19 D*, *2024-12-19 E*, *2025-01-14 2*, *2025-02-14*, *2025-03-05*

### 🖥️ Monitor (Hoare / Signal-and-Continue)
* *2021-05-17*, *2021-12-13 2*, *2022-05-26*, *2022-07-08*, *2022-12-16 B*, *2022-12-16 C*, *2023-02-28*, *2023-04-18*, *2023-06-16*, *2023-07-25*, *2023-10-26*, *2023-11-30*, *2023-12-19 A*, *2023-12-19 B*, *2023-12-19 C*, *2024-02-21 1*, *2024-03-18*, *2024-12-19 A*, *2024-12-19 B*, *2024-12-19 C*, *2024-12-19 D*, *2024-12-19 E*, *2025-01-14 1*, *2025-03-05*, *Compito_Esercitazione*

### 🚦 Processi e Semafori (System V)
* *2022-12-16 A*, *2023-09-28*, *2025-05-05*

---

## 🖧 Sincronizzazione con Macchina Virtuale (UTM / QEMU)

Per trasferire rapidamente i file aggiornati dalla macchina virtuale di sviluppo (Linux) al tuo Mac Host, puoi utilizzare il comando `scp` da un terminale macOS:

```bash
scp -r studente@192.168.64.11:"/home/studente/Scrivania/Sistemi Ope" ~/Desktop/
```

*(Assicurati che la macchina virtuale sia avviata, connessa in rete e con il servizio SSH attivo).*
