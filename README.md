IMPLEMENTAZIONE DI UN SOFTWARE IN C++ PER

SIMULAZIONI DI STORMI

Benedetti Camilla, Dal Fiume Dario Orso, Solieri Laura

January 10, 2023

Abstract

L’obiettivo di questo progetto `e quello di implementare un software in linguaggio C++ per la rappresentazione numerica e grafica di stormi in volo. Il comportamento dei membri dello stormo – i boids – dipender`a dai parametri che il programma accetta in input: parametri di allineamento, separazione, coesione, velocit`a massima, angolo e raggio di campo visivo. Una volta determinati i parametri ottimali, i dati ricavati (velocit`a e distanza medie degli uccelli) possono essere utilizzati per analisi statistiche.

1  Introduzione<a name="_page0_x56.69_y314.50"></a> e cenni storici

Lo studio di sistemi complessi, come gli stormi di uccelli in volo, interessa sia la statistica che la fisica. Il premio Nobel per la fisica 2021, Giorgio Parisi, insieme al gruppo CoBBS, ha contribuito significativamente alla descrizione teorica di questo fenomeno[^1].[ I](#_page0_x73.28_y763.28) dati raccolti evidenziano che la velocit`a di un uccello si modifica solo in relazione a quella dei suoi immediati vicini. La propagazione di un effetto correttivo locale crea visivamente un effetto ordinato globale.

I modelli piu` avanzati per la descrizione della dinamica di uno stormo fanno uso della meccanica statistica, ma si possono ottenere risultati di simulazione realistici anche con poche “regole” appli- cate alla meccanica classica (metodo meno dispendioso come complessit`a di calcolo). Gi`a nel 1986 Craig Reynolds svilupp`o Boids, un simulatore per movimenti coordinati di animali, da poter usare nell’animazione di film o nella computer grafica dei videogiochi[^2].

2  Il nostro modello

L’obiettivo del nostro programma `e rappresentare in un piano bidimensionale, proiezione di uno spazio toroidale, il movimento di uno stormo. La velocit`a di un uccello (chiamato boid, contrazione di “Bird-oid Object”) viene influenzata da tre regole.

- SEPARAZIONE: ogni boid si allontana dagli altri quando `e troppo vicino (ovvero quando la distanza diventa minore della distanza minima ds). La variazione di velocit`a corrispondente `e data dalla formula:

V⃗1 = −s X⃗j − X⃗i se X⃗j − X⃗i  < ds (1)

j=i

Con s parametro di separazione.

- ALLINEAMENTO: ogni boid tende ad allineare la propria velocit`a con quella dei vicini, modi-

  ficando la propria velocit`a in base alla formula:

  

V⃗2 = a n −1 1 V⃗j  − V⃗i (2)

j=i

Con a parametro di allineamento.

- COESIONE: ogni boid tende a spostarsi verso il centro di massa dei boids vicini, cambiando velocit`a secondo la formula:

  

V⃗3 = c 1 X⃗j  − X⃗i (3)

n − 1

j=i

Con c parametro di coesione.

Queste regole si applicano solo tra boids vicini (v. Figura 1).[ Un](#_page1_x56.69_y236.58) boid viene considerato “vicino” o “visibile” quando rientra all’interno del campo visivo di un altro, definito dalla direzione della velocit`a, dall’angolo e dalla distanza massima di visione. L’unica eccezione `e la regola di separazione, per la quale `e sufficiente che il boid sia entro una circon- ferenza di raggio pari a dSeparation ![](Aspose.Words.411713dc-59a4-480a-9e95-0e861522be38.001.png)(la distanza minima di separazione tra due boids).

<a name="_page1_x56.69_y236.58"></a>Figura 1: Rappresentazione di due ![](Aspose.Words.411713dc-59a4-480a-9e95-0e861522be38.002.jpeg)boids e dei relativi campi visivi (in blu e arancione, con angoli verdi). Le cir- conferenze azzurre rappresentano in- vece la distanza di separazione.

3  Implementazione

Nella Sezione [7,](#_page9_x56.69_y138.27) si pu`o trovare un elenco riassuntivo di tutte le funzioni. Di seguito sono rappre- sentati uno schema logico di inclusione delle classi e delle struct utilizzate (Figura 2) e [uno](#_page1_x56.69_y561.70) schema di inclusione dei file (Figura 3[).](#_page2_x56.69_y49.92)

<a name="_page1_x56.69_y561.70"></a>Figura 2: Schema logico![](Aspose.Words.411713dc-59a4-480a-9e95-0e861522be38.003.png) di inclu- sione delle classi e delle struct utiliz- zate.

<a name="_page2_x56.69_y49.92"></a>Figura 3: Schema logico![](Aspose.Words.411713dc-59a4-480a-9e95-0e861522be38.004.png) di inclu- sione dei file di intestazione, im- plementazione e test.

1. Classe<a name="_page2_x56.69_y346.90"></a> Boid

Per rappresentare i boids all’interno di uno spazio euclideo bidimensionale, le coordinate fondamen-

tali sono quelle di posizione e velocit`a, delle quali avremo sia una componente x che una componente y. Definiamo una struct Coordinate formata da due variabili double (x, y) e tutti i relativi operatori (+=, \*=, -=, +, \*, -, ==, !=) secondo le definizioni algebriche (v. Tabella 1 in [Sezione](#_page9_x56.69_y172.25) 7).[ ](#_page9_x56.69_y138.27)operator+ in particolare `e implementato come template. Aggiungiamo inoltre una funzione module per avere la distanza dall’origine o l’intensit`a della velocit`a, in valore assoluto. Usiamo un’altra struct per definire il FOV (Field Of View, ovvero il campo di visione degli uccelli) in

cui il primo parametro `e l’angolo di visione (qui inteso come la met`a dell’angolo di visione complessivo)

e il secondo `e il raggio di distanza massima entro la quale i boids possono vedere gli altri.

A questo punto definiamo la classe Boid (v. Tabella 2 [in ](#_page10_x56.69_y275.69)Sezione 7) [per](#_page9_x56.69_y138.27) rappresentare gli elementi

del nostro stormo. Nella parte privata includiamo un Coordinate position![](Aspose.Words.411713dc-59a4-480a-9e95-0e861522be38.005.png) e un Coordinate velocity~~ . Nella parte pubblica definiamo il costruttore, che prende come input le struct per posizione e velocit`a e le assegna alle variabili private. Seguono le funzioni membro p e v, che permettono di ricavare la posizione e la velocit`a di un boid.

Includiamo quindi distance, che permette di calcolare la distanza tra il boid stesso e qualsiasi altro

boid dato in input (v. Formula 4 [in ](#_page12_x204.25_y479.67)Sezione 7).

La funzione membro findBoid esamina un boid preso in input, e verifica se rientra nel FOV del boid

al quale `e applicata. Quando un boid rientra nel FOV (ovvero la sua distanza dal boid corrente `e minore del raggio e la congiungente tra i boids ricade all’interno dell’angolo), la funzione ritorna un booleano true, in caso contrario, false. Per calcolare la distanza usiamo distance, mentre per l’angolo sfruttiamo la formula inversa del prodotto scalare tra il vettore velocit`a del boid (che indica anche la sua direzione di marcia) e il vettore congiungente i due boids (v. Formula 5 in[ Sezione](#_page12_x176.82_y533.73) 7).

Notiamo che, per avere un FOV che si muove in modo solidale con la direzione della velocit`a del boid, dobbiamo calcolare l’angolo formato dal vettore velocit`a rispetto al sistema di riferimento glob-

ale. La funzione membro direction svolge questo calcolo rispetto al sistema di riferimento di SFML e servir`a per rappresentare la testa del boid (v. Sezione 3.3;[ v. ](#_page4_x56.69_y56.69)Formula 6 in [Sezione](#_page12_x232.99_y588.87) 7).

Per la classe Boid abbiamo definito operator+= per poter successivamente sfruttare l’algoritmo accu- mulate (v. Sezione [3.2).](#_page3_x56.69_y56.69) Infine, abbiamo implementato la funzione bool equalPosition, per verificare se due boids hanno posizioni coincidenti.

2. Classe<a name="_page3_x56.69_y56.69"></a> Flock

Per contenere ordinatamente i parametri di separazione, allineamento e coesione abbiamo creato la struct RuleParameters, composta da tre double: s, a, c. Nello stesso file, la struct Stats serve a raggruppare le informazioni statistiche dello stormo, ovvero le medie di velocit`a e distanza tra i boids, con le relative RMS (Root Mean Square).

La classe Flock aggrega i singoli boids in uno stormo. Nella parte privata abbiamo incluso i valori double const dSeparation ![](Aspose.Words.411713dc-59a4-480a-9e95-0e861522be38.006.png) (che fissa a 30px la minima distanza di separazione tra due boids dello stormo) e Coordinate vLim (il limite superiore al modulo della velocit`a). Gli altri membri sono un std::vector<Boid>![](Aspose.Words.411713dc-59a4-480a-9e95-0e861522be38.007.png) flock~~ , un RuleParameters rulePar e il Coordinate pLim (quest’ultimo definisce![](Aspose.Words.411713dc-59a4-480a-9e95-0e861522be38.008.png)![](Aspose.Words.411713dc-59a4-480a-9e95-0e861522be38.009.png)

il massimo spostamento consentito ai boids entro lo schermo, rispetto all’origine del sistema di riferi- mento). A questi abbiamo aggiunto il char species~~ , che consente di distinguere boids appartenenti a stormi differenti. Di default la specie `e “G”.

Rientrano nella parte privata anche le funzioni per il calcolo delle tre variazioni della velocit`a: vSepa- ration, vAlignment e vCohesion (v. Sezione [1, ](#_page0_x56.69_y314.50)v. Tabella 3[ in](#_page11_x56.69_y118.28) Sezione 7[). ](#_page9_x56.69_y138.27)Nel calcolo di vSeparation compare un

std::runtime~~ error per assicurarsi che il programma fallisca se la distanza tra una qualsiasi coppia di boids `e quasi nulla (<1E3). Si verifica un runtime~~ error quando si inseriscono parametri non otti-

mali per controllare il flock. Il programma deve poter evolvere correttamente con le tre sole regole di separazione, allineamento e coesione, perci`o questa `e un’indicazione per ricercare parametri migliori.

La parte pubblica di Flock contiene il costruttore, che accetta come argomenti un RuleParameters ed i Coordinate che definiscono pLim ![](Aspose.Words.411713dc-59a4-480a-9e95-0e861522be38.010.png) e vLim~~ . Seguono alcune funzioni che consentono di operare sul vector flock~~ : pushBack, con la quale `e possibile ampliare lo stormo, size (che restituisce il numero di boids nello stormo) e back, che consente l’accesso all’ultimo elemento del vettore. Abbiamo definito anche i getters e setters: setPar, setVLim, getSpecies, setSpecies.

La creazione dello stormo `e affidata alla funzione generate, che prende in input un intero n (il numero di boids che si vuole aggiungere) e un random~~ device by reference. Quest’ultimo serve per inizializzare il seed, che viene passato al vero e proprio generatore di numeri pseudo casuali:

std::random~~ default~~ engine, il quale a sua volta assegna ai boids posizioni e velocit`a uniformemente distribuite rispettivamente entro ([50px, pLim~~ .x - 50px], [50px, pLim~~ .y - 50px]) e [-500., 500.].

La funzione `e strutturata secondo un for loop e, al termine di ogni ciclo, esegue un pushBack del boid generato. Prima dell’assegnazione di posizione e velocit`a al nuovo boid, un ciclo while verifica che la posizione generata non rientri all’interno della distanza di separazione degli altri boids del flock. Se

la generazione di uno stesso boid fallisce 5 volte, viene lanciato un errore che comunica all’utente che probabilmente il numero di boids sul piano `e troppo elevato.

La funzione valControl regola la posizione e la velocit`a di ogni boid, mantenendole entro pLim ![](Aspose.Words.411713dc-59a4-480a-9e95-0e861522be38.011.png) e vLim~~ : se la velocit`a `e maggiore (in modulo) della velocit`a limite, viene riportata al valore vLim~~ ; se la posizione supera di qualche Pixel uno dei due estremi dell’intervallo [0, pLim~~ ] in verticale o in orizzontale, viene resettata al valore dell’estremo opposto con l’aggiunta del numero di pixel di cui ha sconfinato (cos`ı si ricrea visivamente nei boids un effetto Pacman[^3]).

La funzione evolve accetta come argomenti un FOV e il double delta~~ t, che definisce l’intervallo di tempo durante il quale i boids cambiano posizione e velocit`a all’interno di un ciclo.

Il primo passo `e la creazione di un vettore copia costante a partire da flock~~ . In questo modo, i calcoli vengono eseguiti sui valori originali e solo al termine le posizioni e le velocit`a vengono sovrascritte (effetto di simultaneit`a dell’aggiornamento del flock). Si procede con un for loop che scorre l’intero flock e ricava le tre variazioni di velocit`a dovute a separazione (v1), allineamento (v2) e coesione![](Aspose.Words.411713dc-59a4-480a-9e95-0e861522be38.012.png)

(v3). Queste, sommate alla velocit`a originaria, restituiscono la nuova velocit`a vNew.

Piu` semplice `e il calcolo della posizione aggiornata pNew, ottenuta dalla somma della posizione prece- dente con il prodotto tra delta~~ t e la velocit`a originaria.

La funzione state restituisce by const reference il vector flock~~ .

La funzione statistics calcola e restituisce in una struct Stats i valori medi di velocit`a e distanza, con relative deviazioni standard (v. Formula 7, [Tab](#_page12_x169.34_y653.77)ella 5, [in ](#_page12_x56.69_y444.46)Sezione 7).

3. Main.cpp

<a name="_page4_x56.69_y56.69"></a>Il file contiene le istruzioni per la gestione dell’interfaccia grafica, oltre alle funzioni libere mainEvolve e askPar. mainEvolve esegue piu` volte (numero di steps per evolution) la funzione evolve sul flock che viene passato e aggiorna le posizioni e le velocit`a dei singoli boids. Al termine dell’operazione restituisce lo stato del flock, con la funzione state.

askPar contiene invece le istruzioni per prendere in input da terminale i dati necessari alla creazione di uno stormo. Per mezzo del char option, si distinguono il caso in cui l’utente accetti i valori di default

e quello in cui invece voglia inserire parametri personalizzati. In seguito la funzione chiede il numero

di boids da generare per un nuovo flock.

All’interno della funzione main per prima cosa viene inizializzato un generatore random per il seed ed `e creato (o ricreato) il file di output “databoids.txt” (v. Figura [9 ](#_page7_x394.01_y542.62)in Sezione [5.2.4),](#_page7_x56.69_y496.32) sul quale verranno salvati i dati dello stormo.

Sono quindi definite tre costanti per il tempo e la velocit`a di evoluzione: delta~~ t, fps (frames per second) e stepsPerEvolution. La prima `e un sf::Time del valore di 1 ms. Le altre due sono di tipo

int, pari rispettivamente a 60 fps[^4] [e ](#_page4_x73.28_y748.90)a 1000/fps. Con questi dati, in un ciclo completo della funzione mainEvolve avvengono 16 evolve del flock mentre la funzione state restituisce lo stato aggiornato del flock soltanto ogni 16 millisecondi, garantendo al tempo stesso una buona accuratezza grafica e di calcolo, insieme ad una minor pesantezza del file databoids.txt.

Due variabili double sono inizializzate al 90% delle dimensioni dello schermo del dispositivo su cui

si fa girare il programma, per poter poi creare una finestra grafica di dimensioni adeguate.

Vengono gi`a creati due flock: il primo richiede subito, prima di essere definito, parametri in input da terminale, mentre il secondo resta in attesa dei comandi successivi. Abbiamo ritenuto che avere due flock fosse sufficiente per testare il loro comportamento. I due stormi non interagiranno tra loro, ma

solo con altri boids della stessa “specie”.

Le righe successive del codice riguardano l’aspetto e i comandi della finestra grafica. I boids del primo flock saranno triangoli isosceli verdi (specie G), mentre quelli del secondo flock saranno blu (specie B). Grazie alla funzione direction della classe Boid (v. Sezione 3.1[), i ](#_page2_x56.69_y346.90)triangoli si orientano nello spazio seguendo la direzione della velocit`a dei boids. In seguito, si apre la finestra grafica e si definiscono gli eventi possibili con cui si potr`a interagire al run time.

Mentre i parametri del primo flock sono fissati, i parametri del secondo possono essere modificati ogni volta che viene premuto il tasto F; in questo caso, inoltre, il numero di boids richiesto di volta in volta va a sommarsi a quello dei boids blu gi`a presenti. Per tenere conto del numero di interazioni con i tasti A ed F, abbiamo creato le variabili int n ed m, il cui valore viene riportato nel titolo della finestra insieme al numero totale di boids, per l’ultimo stormo modificato.

4  Istruzioni
1. Download e compilazione

Per compilare i file di test (boid.test.cpp, flock.test.cpp e vel.dist.test.cpp) `e necessario aver scari- cato il file doctest.h. Si pu`o scaricare utilizzando il comando:

curl https://raw.githubusercontent.com/giacomini/pf2021/main/code/doctest.h -o

doctest.h

Per compilare ed eseguire il programma main.cpp `e necessario scaricare la libreria grafica SFML. Su Ubuntu 20.04 `e sufficiente eseguire il comando:

sudo apt install libsfml-dev

La compilazione avviene tramite CMake[^5],[ secondo](#_page4_x73.28_y759.86) i comandi contenuti nel file CMakeLists.txt, e deve essere differenziata in base alla natura del file da compilare:

- Per compilare i file di test si usa la modalit`a Debug con il comando:

  cmake -S . -B <nomecartella> -DCMAKEBUILDTYPE=Debug Nella![](Aspose.Words.411713dc-59a4-480a-9e95-0e861522be38.013.png)![](Aspose.Words.411713dc-59a4-480a-9e95-0e861522be38.014.png) cartella <nomecartella> verr`a cos`ı creato l’eseguibile flock.t

- Per compilare il main si usa la modalit`a Release, con il comando:

  cmake -S . -B <nomecartella> -DCMAKEBUILDTYPE=Release Nella![](Aspose.Words.411713dc-59a4-480a-9e95-0e861522be38.015.png)![](Aspose.Words.411713dc-59a4-480a-9e95-0e861522be38.016.png) cartella <nomecartella> verr`a creato l’eseguibile flock-sfml

Con l’opzione -fsanitize=address, il compilatore segnala un memory leak alla chiusura della fines- tra. Questo `e dovuto esclusivamente alla libreria grafica SFML e comunque non compromette la funzionalit`a del programma.

Il codice`e stato formattato con il canone di clang-format. Quest’ultimo si pu`o scaricare con il comando:

clang-format --dump-config -style=google > .clang-format

2. Comandi in input del programma principale

Per prima cosa viene chiesto all’utente di specificare se voglia procedere con i parametri di default

e, in caso di risposta negativa, vengono richiesti da terminale i parametri di separazione, allineamento, coesione, angolo, raggio e velocit`a limite dei boids (in questo ordine). I valori accettati nel caso dei primi tre sono decimali tra 0 e 1, per i restanti sono interi positivi inferiori o uguali rispettivamente a π, 300, 1000. Per ottenere un buon flock consigliamo di tenere s alla prima cifra decimale, mentre

a e c devono essere di un ordine di grandezza inferiore (ad esempio {0.7 0.05 0.03}). I valori vanno inseriti con il punto decimale e uno spazio tra ogni parametro e il successivo. Una volta premuto invio,

il programma prende in input il numero di boids da generare nel primo flock. Premendo nuovamente invio, si apre la finestra grafica e si pu`o cominciare ad osservare lo stormo.

Premendo il tasto A si crea un pallino rosso, che scompare al rilascio, evidenziando la posizione di

un nuovo boid del primo flock. Per riempire il secondo flock basta premere il tasto F (una sola volta)

e inserire i parametri richiesti dal terminale. Premendo nuovamente il tasto F `e possibile aggiungere piu` boids alla volta e modificare i parametri di tutto il secondo stormo.

Premendo la X in alto a destra la finestra viene chiusa e il programma terminato. Un metodo di chiusura alternativa sfrutta la pressione del tasto Q.

NOTA: i tasti A, F e Q devono essere premuti con la finestra grafica aperta; la loro pressione a livello

del terminale di Ubuntu non produce alcun effetto.

5  Discussione dei risultati
1. Testing

Abbiamo suddiviso i test in tre files: boid.test.cpp, che serve a testare la classe Boid, flock.test.cpp, che esegue controlli sulle funzioni della classe Flock e infine vel.dist.test.cpp che verifica le formule per il calcolo della velocit`a e della posizione dello stormo (con relative RMS).

-lsfml-window -lsfml-system

- per i test: g++ -Wall -Wextra -fsanitize=address vel.dist.test.cpp flock.test.cpp boid.test.cpp

NOTA: una volta accertata la correttezza del codice `e consigliabile eseguire una compilazione meno dispendiosa aggiun- gendo le opzioni -O2 (oppure -O3) e -g0.

1. Boid.test.cpp

Gli operatori sono costruiti con la definizione algebrica e funzionano come previsto, come eviden- ziato dai test numerici.

Nei primi tre test case si verifica che l’assegnazione dei valori nella classe Boid sia corretta. Si testano anche il calcolo della distanza tra boids e la funzione findBoid, cos`ı da controllare che permetta di vedere solo i boids che si trovano effettivamente nel campo visivo.

In un altro test case verifichiamo che la funzione direction calcoli correttamente l’angolo del vettore velocit`a di ogni boid.

2. Flock.test.cpp

Il primo test si effettua su un singolo boid inserito nel flock, per verificare l’assegnazione di posizione e velocit`a e per testare evolve. Poi si procede con flock di due e tre boids, valutando se rientrano o meno nel campo visivo e calcolando cos`ı le componenti di velocit`a. Si deve tenere conto anche del comportamento di teletrasporto ai bordi. Gli stormi si comportano come previsto.

Prendendo un flock con un singolo boid, abbiamo verificato che se il boid esce dalla finestra dal lato destro, rientra subito dal lato sinistro (e viceversa), mentre se supera la coordinata massima del bordo superiore, rientra da quello inferiore (e viceversa). Abbiamo testato anche alcune combinazioni di due casi.

3. vel.dist.test.cpp

Le formule per velocit`a media, posizione media e relative deviazioni standard sono state testate separatamente, nel file vel.dist.test.cpp. Abbiamo convertito esplicitamente in double gli int coinvolti nelle operazioni, affinch´e i calcoli non risultassero troppo approssimati.

2. Esecuzione
1. Esempio 1: Creazione di uno stormo con tasto A

Figura 4: Creazione di un primo stormo di size nulla![](Aspose.Words.411713dc-59a4-480a-9e95-0e861522be38.017.png)![](Aspose.Words.411713dc-59a4-480a-9e95-0e861522be38.018.png) e parametri di default {0.7, 0.05, 0.03}. Aggiunta di boids con il tasto A e formazione di uno stormo coeso.

2. Esempio 2: Aggiunta di un nuovo stormo con tasto F

Figura 5: Creazione del secondo stormo![](Aspose.Words.411713dc-59a4-480a-9e95-0e861522be38.019.png)![](Aspose.Words.411713dc-59a4-480a-9e95-0e861522be38.020.png) blu, aggiunta di nuovi boids al secondo stormo con modifica dei parametri.

3. Esempio 3: Parametri non ottimali

I parametri di default sono {0.7, 0.05, 0.03} e danno buoni risultati. Oltre alla verifica di std::runtime~~ error, per capire quali parametri sono ottimali si pu`o direttamente osservare il com- portamento dello stormo sulla finestra grafica.

Un parametro di separazione molto basso rispetto a      quelli di coesione e allineamento {0.01 0.5 0.5} porta alla sovrapposizione![](Aspose.Words.411713dc-59a4-480a-9e95-0e861522be38.021.png)![](Aspose.Words.411713dc-59a4-480a-9e95-0e861522be38.022.png) quasi immediata dei boids e dunque ad     una rapida terminazione del programma. Al contrario,

un parametro di separazione molto elevato {0.9 0.001 0.002} induce visivamente urti totalmente elastici tra i boids.

Figura 6: s << a,c

Un parametro di allineamento molto basso rispetto

a quelli di coesione e separazione pu`o determinare la ![](Aspose.Words.411713dc-59a4-480a-9e95-0e861522be38.023.png)![](Aspose.Words.411713dc-59a4-480a-9e95-0e861522be38.024.png)formazione di nuclei ben distinti all’interno del medes- imo stormo. Al contrario, valori molto alti portano a cambi di direzione estremamente bruschi non appena un boid con vettore velocit`a differente entra nel campo vi- sivo.

Figura 7: a << c,s

Un parametro di coesione molto elevato rispetto

a quelli di separazione e allineamento pu`o far s`ı che![](Aspose.Words.411713dc-59a4-480a-9e95-0e861522be38.025.png)![](Aspose.Words.411713dc-59a4-480a-9e95-0e861522be38.026.jpeg) alcuni boids si ritrovino immobilizzati gli uni contro gli altri al limite della distanza di sepa- razione o a proseguire al di sotto della stessa.

Parametri di coesione estremamente bassi {0.7 0.03 0.001} rendono invece difficoltosa la creazione di uno

Figura 8: c >> s,a

stormo.

4. Esempio<a name="_page7_x56.69_y496.32"></a> 4: Come usare i dati del file

Dal file databoids.txt si ottiene una colonna unica di dati, separati da spazi. Nell’ordine, vengono stampati:

<a name="_page7_x394.01_y542.62"></a>10

- la specie dello stormo considerato
- la velocit`a media dei boids dello stormo
- la deviazione standard di tale velocit`a
- la distanza media tra i boids dello stormo
- la deviazione standard di tale distanza

L’utilizzo della virgola come separatore tra le colonne agevola la lettura dei dati, oltrech´e la loro disposizione all’interno di un foglio

di calcolo. Per una corretta gestione dei dati si consiglia di ordinarli alfabeticamente (per separare B e G) su un foglio di calcolo e solo

in un secondo momento suddividere il testo in colonne. Una volta separate le colonne di dati, si possono facilmente creare i grafici per velocit`a e distanza in funzione del tempo. Qui riportiamo l’esempio

dei dati ottenuti dal singolo stormo verde, con parametri {0.7, 0.05, 0.03} (vv. Figura [10a e](#_page8_x56.69_y133.56) Figura [10b).](#_page8_x56.69_y133.56)

Figura 9: Una porzione dei dati stampati sul file databoids.txt.

![](Aspose.Words.411713dc-59a4-480a-9e95-0e861522be38.027.jpeg)

11

![](Aspose.Words.411713dc-59a4-480a-9e95-0e861522be38.028.png)

1) Figura<a name="_page8_x56.69_y133.56"></a> 10: In blu si pu`o notare l’andamento nel tempo della velocit`a media: i bruschi cali sono legati all’aggregazione di gruppi di boids allo storno principale. In arancione `e invece graficata la deviazione standard della velocit`a nel tempo: i picchi di imprecisione corrispondono ai momenti di aggregazione dei boids. Trascorso un tempo sufficientemente lungo (circa 23.7 s, per un campione di 10 boids) la velocit`a media si assesta sul valore limite e la deviazione standard si approssima allo zero.

![](Aspose.Words.411713dc-59a4-480a-9e95-0e861522be38.029.png)

2) Figura 11: In blu `e rappresentato l’andamento nel tempo della distanza media: trascorsi i primi istanti, nei quali i boids sono generati randomicamente e risultano quindi notevolmente dispersi sullo schermo, la curva assume un andamento regolare caratterizzato dalla presenza di picchi periodici, corrispondenti ai momenti in cui i boids subiscono l’effetto Pacman. La curva della deviazione standard, in arancione, segue un andamento simile.
6  Conclusioni

I test condotti e gli esempi di output evidenziano che il software riesce a simulare abbastanza realisticamente il comportamento di stormi in volo. In particolare, i parametri ottimali per osservare un comportamento di volo lineare sono intorno a {0.7, 0.05, 0.03}.

7  Appendice

<a name="_page9_x56.69_y138.27"></a><a name="_page9_x56.69_y172.25"></a>Tabella 1: STRUCT Coordinate![](Aspose.Words.411713dc-59a4-480a-9e95-0e861522be38.030.png)

|FUNZIONI MEMBRO||
| - | :- |
|auto& operator+=(Coordinate const& rhs)|Operatore di assegnazione composta += tra Co- ordinate.|
|auto& operator\*=(double const value)|Operatore di assegnazione composta \*= tra Co- ordinate.|
|auto& operator-=(Coordinate const& rhs)|Operatore di assegnazione composta -= tra Co- ordinate.|
|FUNZIONI LIBERE||
| - | :- |
|template <class T>|Dichiarazione template.|
|inline T operator+(T const& lhs, T const& rhs)|Template per operatore di assegnazione +.|
|inline auto operator\*(double const d, Coordi- nate const c1)|Operatore di assegnazione \* tra double e Coor- dinate.|
|inline auto operator-(Coordinate const c1, Co- ordinate const c2)|Operatore di assegnazione - tra Coordinate.|
|inline auto operator==(Coordinate const c1, Coordinate const c2)|Operatore di confronto == tra Coordinate.|
|inline auto operator!=(Coordinate const c1, Co- ordinate const c2)|Operatore di confronto != tra Coordinate.|
|inline double module(Coordinate const c1)|Restituisce il modulo di un Coordinate.|

Tabella riassuntiva per le funzioni della struct Coordinate.

<a name="_page10_x56.69_y275.69"></a>Tabella 2: CLASSE Boid![](Aspose.Words.411713dc-59a4-480a-9e95-0e861522be38.031.png)

|FUNZIONI MEMBRO||
| - | :- |
|Boid(Coordinate p, Coordinate v)|Costruttore.|
|Coordinate p() const|Ritorna la posizione del boid.|
|Coordinate v() const|Ritorna la velocit`a del boid.|
|double distance(Boid const& b) const|Ritorna la distanza tra il boid corrente ed un secondo oggetto b di tipo Boid.|
|bool findBoid(Boid const& b, FOV const& fov) const|Verifica la presenza di un oggetto b di tipo Boid nel campo visivo del boid corrente.|
|double direction() const|Ritorna l’angolo α compreso tra la verticale e la direzione del vettore velocit`a del boid. Il calcolo dell’angolo `e adattato al sistema di coordinate della finestra grafica di SFML.|
|auto operator+=(Boid const& b)|Somma al boid corrente un altro oggetto b di tipo Boid.|
|FUNZIONI LIBERE||
| - | :- |
|bool equalPosition(Boid const& b1, Boid const& b2)|Verifica l’uguaglianza tra le posizioni di due boids.|

Tabella riassuntiva per le funzioni della classe Boid.

<a name="_page11_x56.69_y118.28"></a>Tabella 3: CLASSE Flock![](Aspose.Words.411713dc-59a4-480a-9e95-0e861522be38.032.png)

|FUNZIONI MEMBRO PRIVATE||
| - | :- |
|auto vSeparation (Boid const& b, std::vector<Boid> const& flock) const|Se trova un boid b al di sotto di dSeparation~~ , calcola la velocit`a di separazione (v. Intro- duzione).|
|auto vAlignment (Boid const& b, std::vector<Boid> const& flock, FOV const& fov) const|Se trova un boid b all’interno del FOV del boid corrente, calcola la velocit`a di allineamento (v. Introduzione).|
|auto vCohesion(Boid const& b, std::vector<Boid> const& flock, FOV const& fov) const|Se trova un boid b all’interno del FOV del boid corrente, calcola la velocit`a di coesione (v. In- troduzione).|
|FUNZIONI MEMBRO PUBBLICHE||
| - | :- |
|Flock(RuleParameters par, Coordinate const pLim, double vLim = 500.)|Costruttore.|
|void pushBack(Boid const& b)|Aggiunge un Boid b al vettore di boids.|
|int size()|Restituisce la dimensione del vettore di tipo Boid.|
|Boid const& back()|Restituisce una referenza all’ultimo elemento del vettore di boids.|
|void setPar(RuleParameters par)|Assegna i parametri par al Flock corrente.|
|void setVLim(double vLim)|Imposta la velocit`a limite dello stormo.|
|char getSpecies() const|Ritorna la specie del Flock corrente.|
|void setSpecies(char c)|Imposta la specie del Flock corrente ad un char c.|
|void generate(int n, std::random~~ device& seed- Gen)|Genera randomicamente e aggiunge al vettore di tipo Boid un numero n di boids. Nella funzione sono utilizzati i generatori std::random~~ device e std::default~~ random~~ engine (il primo, accettato in input, per fornire il seed al secondo, che `e us- ato per la generazione vera e propria dei boids).|
|auto valControl(Coordinate& vel, Coordinate& pos)|Controlla i valori di posizione e velocit`a di un boid per l’effetto Pacman.|
|void evolve(double const delta~~ t, FOV const& fov)|Operando su un vettore copia, esegue il cal- colo delle nuove velocit`a e posizioni dei boids del Flock rispettando le regole di separazione, allineamento e coesione, applicando l’effetto Pacman e imponendo la velocit`a limite.|
|std::vector<Boid> const& state() const|Ritorna il vettore di boids aggiornato.|
|Stats statistics() const|Calcola velocit`a e distanza media dei boids, con relative deviazioni standard, e le restituisce in una struct Stats.|

Tabella riassuntiva per le funzioni della classe Flock.

Tabella 4: File main.cpp![](Aspose.Words.411713dc-59a4-480a-9e95-0e861522be38.033.png)

|FUNZIONI LIBERE||
| - | :- |
|auto mainEvolve (Flock& flock, int stepsPerEv- olution, sf::Time delta~~ t, FOV const& fov)|Esegue un evolve sul flock che viene passato in argomento come reference e ne restituisce lo stato, tramite il vector di boids.|
|int askPar (RuleParameters& par, FOV& fov, double& vLim)|Chiede all’utente i parametri s, a, c, angolo e raggio del FOV, velocit`a limite. In base all’input da terminale li imposta ai valori di default o a quelli personalizzati. Inoltre legge il numero di boids da generare (nBoids) e lo restituisce fuori dallo scope.|

Tabella riassuntiva per le funzioni delfile main.cpp.

<a name="_page12_x56.69_y444.46"></a>Tabella 5: Formule![](Aspose.Words.411713dc-59a4-480a-9e95-0e861522be38.034.png)

|<a name="_page12_x204.25_y479.67"></a>distance = (xi − xj )2 + (yi − yj )2 (4)![](Aspose.Words.411713dc-59a4-480a-9e95-0e861522be38.035.png)|
| - |
|<p><a name="_page12_x176.82_y533.73"></a>Vx ·(xi − xj ) + Vy ·(yi − yj )</p><p>angle = arccos |V⃗| · distance (5)</p>|
|<p><a name="_page12_x232.99_y588.87"></a>Vx 180</p><p>α = arccos |V⃗| · π (6)</p>|
|<p><a name="_page12_x169.34_y653.77"></a>RMS = σ = Ni=1 (xi − x)2 =  N · x2 − x2 (7)![](Aspose.Words.411713dc-59a4-480a-9e95-0e861522be38.036.png)![](Aspose.Words.411713dc-59a4-480a-9e95-0e861522be38.037.png)![](Aspose.Words.411713dc-59a4-480a-9e95-0e861522be38.038.png)![](Aspose.Words.411713dc-59a4-480a-9e95-0e861522be38.039.png)![](Aspose.Words.411713dc-59a4-480a-9e95-0e861522be38.040.png)</p><p>N − 1 N − 1</p>|

Tabella contenente le formule di maggior complessit`a utilizzate nel programma.
15

[^1]: <a name="_page0_x73.28_y774.24"></a><a name="_page0_x73.28_y763.28"></a>[CoBBS – Collective Behaviour in Biological Systems – CNR-ISC](https://www.isc.cnr.it/groups/cobbs/)
[^2]: <http://www.red3d.com/cwr/boids/>
[^3]: <a name="_page3_x73.28_y761.56"></a> Con riferimento all’omonimo videogioco, abbiamo cos`ı definito l’effetto di teletrasporto sui boids. Questo `e stato fatto per non perdere i boids oltre i limiti della finestra grafica.
[^4]: <a name="_page4_x73.28_y759.86"></a><a name="_page4_x73.28_y748.90"></a> E` il valore standard per PC, costituendo in genere il rapporto ideale tra resa e risorse.
[^5]: In assenza di CMake, i file compilano con le seguenti opzioni:

    • per il main: g++ -Wall -Wextra -fsanitize=address main.cpp flock.cpp boid.cpp -lsfml-graphics
