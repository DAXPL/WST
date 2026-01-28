How to setup docker:
1. Install [Docker Desktop](https://docs.docker.com/desktop/setup/install/windows-install/) (Restart is required after)
2. Open terminal in the Unity project
3. If [make](https://sourceforge.net/projects/gnuwin32/files/make/3.81/make-3.81.exe/download) is installed in the system:
    ```bash
    make build
    ```
    if not, then:
    ```bash
    docker compose build
     ```


Training:
1. Open Unity
2. Open scene with you environment that will run training
4. Open terminal in the Unity project
5. If [make](https://sourceforge.net/projects/gnuwin32/files/make/3.81/make-3.81.exe/download) is installed in the system:
    ```bash
    make train
    ```
    if not, then:
    ```bash
    docker compose up
     ```
6. When large text `UNITY` is visible in the terminal, run the scene inside Unity

For other available commands look inside `Makefile`
