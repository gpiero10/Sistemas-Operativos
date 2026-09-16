***Examen 1***

---

Ejercicio 3
---

(a) 
Multilevel Queue

    q0 = RT (escaneo y estabilizacion)
    q1 = FCFS (tareas prioridad urgentes)
    q2 = FCFS (tareas prioridad media)
    q3 = FCFS (tareas prioridad baja)
    q4 = RR (logs)

Multilevel queue sin aging y con desalojo porque el dron ejecuta distintos procesos con prioridades distintas entre si, ademas no requiero aging ya que 
son tareas con prioridades escencialmente estaticas, y por ultimo con desalojo para que siempre que una tarea de q0 este disponible sea ejecutada.

En la q0 pongo las tareas RT ya que el dron periodicamente debe escanear el entorno y ser estable, esto son funciones criticas por lo tanto son las mas prioritarias. Aca se implementa EDF.

En la q1, q2 y q3 pongo las tareas disparadas remotamente que son importantes ya que el operador esperara respuestas preferiblemente rapidas acorde al orden de pedidos. En las 3 colas se usa FCFS, que en definitiva priorizada el orden de llegada.
    
En la q4 pongo los logs ya que son procesos que no son sensibles a demoras ya que estan pensados para correr en "2do plano" pero eventualmente se ejecutaran si el dron no tiene tareas en las colas anteriores. Aca elijo un Round-Robin ya que todas las tareas log tienen la misma importancia.

(b)

t_llegada = 0ms

Q0: T1 Sensado              tiempo de ejecucion: 1ms, periodo: 5ms,  deadlineRelativo: 5ms
    T2 Estabilizacion       tiempo de ejecucion: 2ms, periodo: 10ms, deadlineRelativo: 7ms

Q4: T3 actualizacion GPS    tiempo de ejecucion: 1ms, periodo: 20ms, deadlineRelativo: 20ms

t_llegada = 5ms
Q1: T4 Comunicacion con central de mando 2ms, p: 25ms, d: 20ms (el enunciado no aclata la prioridad de este proceso, lo mando a urgente)


Diagrama de Gannt

    ms   |   1  |  2 |  3  |  4 |  5  |  6 |  7 |  8 |  9 |  10 | 11 | 12 | 13 | 14 | 15 | 16 | 17 | 18 | 19 | 20 | 21 | 22 | 23 | 24 | 25 | 26

    t    |   T1 | T2 |  T2 | T3 | T4  | T1 | T4 |  - |  - |  -  | T1 |  - | T2 | T2 | -  | T1 | -  | -  |  - | -  | T1 | -  | -  | T2 | T4 | T3    


***Otro Parcialito (SO 1c-1R2025)***
---

Ejercicio 1
---

// Codigo SERVER
    
    #define forn(i,n) for(int i = 0; i < n; i++)
    int n; // asumo, es necesario
    
    struct notaAlumno {
        int nota;
        char lu[5];
        
    } alumno_t;

    void trabajoInfantil(int id, int cliente_fd)
    {
        int materia;
        read(cliente_fd, &materia, sizeof(int));
        
        Acta acta = crearActa(materia);
        alumno_t chabon;

        while(read(cliente_fd, &chabon, sizeof(chabon))>0)
        {
           agregarNota(acta, chabon.lu, chabon.nota);
        }
        cerrarActa(acta);
        
        close(cliente_fd);
        return;
    }

    int main_servidor()
    {
        int servidor_fd, cliente_fd;
        struct sockaddr_un direccion;
        socklen_t addrlen = sizeof(direccion);
        direccion.sin_family = AF_UNIX;
        strcpy(direccion.sum_path, "unix_socket");
        unlink(direccion.sum_path);

        servidor_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    
        bind(servidor_fd, &direccion, sizeof(direccion));
        listen(servidor_fd, N);
        
        for(;;)
        {
            cliente_fd = accept(server_fd, NULL, NULL);
    
            pid_t hijo = fork();
            if (hijo == 0)
            {
                close(servidor_fd);
                trabajoInfantil(i, cliente_fd);
                exit(EXIT_SUCCESS);
            }
            else
            {
                close(cliente_fd);
            }
        }
    
        //forn(i,n) wait(NULL);
    
        exit(EXIT_SUCCESS);
    }




// Codigo Docente

    int materia, K; // inicializados por alguien
    alumno_t* alumnos[K];

    struct notaAlumno {
        int nota;
        char lu[5];
    } alumno_t;

    int main_docente()
    {
        struct sockaddr_un servidor;
        servidor.sun_family = AF_UNIX;
        strcpy(servidor.sum_path, "unix_socket");
        int sock = socket(AF_UNIX, SOCK_STREAM, 0);

        connect(sock, &servidor, sizeof(servidor));
    
        write(sock, &materia, sizeof(int)); // indico mi materia
    
        forn(i,K)
        {
            write(sock, alumnos[i], sizeof(*alumnos[i])); // mando *notaAlumno
        }
    
        close(sock);
        exit(EXIT_SUCCESS);
    }


*** Ejercicio 2 ***
---
    
    // Variables Globales
    int M,N; // Propios de la consigna, los asumo
    semaforo mutex = sem(1);
    semaforo barrera = sem(0);
    semaforo limiteSensores = sem(M);
    
    int resultadosListos = 0;
    
    void procesoCentral()
    {
        while (1)
        {
            barrera.wait()
            consolidarDataSensores();
        }
    }
    
    void procesoAsociadoASensor()
    {
        while(1)
        {
            limiteSensores.wait();
            preprocDataSensor();
            limiteSensores.signal();
    
            mutex.wait();
            resultadosListos ++;
            if (resultadosListos == N) barrera.signal(); 
            mutex.signal();
        }
    
    }

*** Ejercicio 3 ***
---
(a) Falso. Ya que nos mencionan que entre los usuarios hay diferencias de prioridad que se deben respetar por lo tanto no pueden estar en la misma cola.

(b) Falso.  Si bien deben separarse en colas de prioridad, no es cierto que se use FIFO para elegir que user recibe cpu ya que este scheduler debe asegurar que la distribucion de la cpu sea justa y ademas evitar que un proceso tenga mas tiempo de cpu solo por lanzar mas procesos.

(c) Verdadero. Aunque nos aseguran que TODO user usara la CPU de manera justa, y ademas que el scheduler cuenta con metricas que indican el uso de cpu por cada user para lograr esto, no puedo asegurar que usen aging para evitar starvation.
