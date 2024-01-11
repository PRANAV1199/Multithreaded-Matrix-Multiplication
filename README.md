
# Master Program Operation:
        Reads input matrices from the specified file.
        Calculates the bracketing of the task and prepares for distribution.
        Listens for client connections on the specified IP address and port.
        Distributes portions of the job to connected clients.

# Client Program Operation:
        Connects to the master server using the provided IP address and port.
        Receives its portion of the job from the master according to the predefined protocol.
        Performs multi-threaded matrix multiplication on its assigned matrices.
        Sends the result back to the master and exits.

# Master Program (Post-Client Processing):
        Receives results from all clients and stores them in the appropriate data structure.
        Uses a multi-threaded matrix multiplication routine to compute the final result matrix 𝑌.
