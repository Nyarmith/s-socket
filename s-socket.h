// TODO: Add fancy header with authorship and licensing info, flags descriptions under fns that accept flags


// ---------------------
// ---- Address API ----
// ---------------------


/**
 * The address API just consists of the AddrInfo type and a few helpers
 * to encapsulate resolved address info a cross-platform way
 */
struct AddrInfo;

/**
 * Populates AddrInfo object based on args
 *
 * @param host pointer to null-terminated hostname or ip
 * @param port port number we want to use for host
 * @param out a pointer to an AddrInfo object that this function will populate
 * @return zero on success
 *
 * Some examples:
 * setaddrinfo("www.google.com", 80, &request);
 * setaddrinfo("8.8.8.8", 53, &dnsInfo);
 * setaddrinfo("0.0.0.0", 8080, &serverInfo);
 */
int setaddrinfo(char *host,
                size_t port,
                struct AddrInfo *out);

/**
 * Prints resolved ip of an AddrInfo object, useful for checking out the ip of
 * connecting clients. Returns a pointer to a null-terminated string
 *
 * @return null-terminated string on success, NULL on failure
 */
char* gethost(struct AddrInfo *in);

/**
 * Returns port set in AddrInfo object
 *
 * @return port on success, negative values for failure
 */
int getport(struct AddrInfo *in);


// -----------------
// ---- UDP API ----
// -----------------

/**
 * The UDP API fairly straightforward, and consists of a blind send function,
 * and a recv function that operates on a bound socket. UDP provides no guarantees
 * of delivery or duplication protection, so we're basically spamming packets to a
 * network interface
 */
struct UDPSocket;

/**
 * Creates and returns a socket file descriptor.
 * @return If the system fails in socket creation, we return NULL
 */
UDPSocket* udp_mksocket();

/**
 * Sends a buffer msgbuf of size msglen to destInfo over socket sock
 * @param sock pointer to a valid socket
 * @param destInfo pointer to a populated description of our destination
 * @param msgbuf pointer to the data we're sending
 * @param msglen length of the message we're sending
 * @return bytes sent, negative on failure
 */
int udp_send(struct UDPSocket *sock,
                struct AddrInfo *destInfo,
                void *msgbuf,
                size_t msglen,
                size_t flags);

/**
 * udp_bind binds a UDPSocket to a local address and port, which is a prerequisite to udp_recv
 * @param sock valid socket created with udp_mksocket()
 * @param hostInfo description of the interface we're binding
 */
int udp_bind(struct UDPSocket *sock,
             struct AddrInfo *hostInfo);

/**
 * udp_recv blocks until it receives data on the bound socket
 * @param sock a socket bound to a local interface
 * @param msgbuf pointer to the data we're sending
 * @param buflen length of the data we're sending
 * @param flags mods on send
 * @param src pointer to an AddrInfo that, if populates, will give us the destination address
 *        but can also be NULL
 * @return bytes received, less than zero indicates error
 */
int udp_recv(struct UDPSocket *sock,
             void *msgbuf,
             size_t buflen,
             size_t flags,
             struct AddrInfo *out);

/**
 * Closes a udp socket when you're done with it
 * @return non-zero on failure
 */
int udp_free(TCPSocket* sock);


// -----------------
// ---- TCP API ----
// -----------------

/**
 * The TCP API is more complicated than the UDP API; instead of just sending packets to a destination, we
 * maintain an open session with a client and are guaranteed that packets are received in the same order that
 * they are sent
 */
struct TCPSocket;

TCPSocket* tcp_mksocket();
/**
 * tcp_bind binds a TCPSocket to a local address and port, which is a prerequisite to tcp_listen_accept
 */
int tcp_bind(struct UDPSocket *sock,
                struct AddrInfo *hostInfo);

/**
 * tcp_listen listens for incoming connections on a bound socket and then accepts them if we are still
 * within backlog active connections. Returns zero on success. If we succeed, we are furnished
 * with the client address, and an active socket to the client.
 *
 * The client socket is ultimately what we use to tx/rx over tcp, with the bound socket acting as a "daemon" socket
 */
int tcp_listen(struct TCPSocket *sock,
               int backlog, 
               struct TCPSocket *client,
               struct AddrInfo *clientInfo);


/**
 * connect is like bind() but for a client; it establishes a maintained connection that lets us continually
 * send and receive data without interruption.
 *
 * Returns zero on success.
 */
int tcp_connect(struct TCPSocket *sock,
                struct AddrInfo *dest);

/**
 * sends buffer msgbuf of size buflen over active socket sock
 */
int tcp_send(struct TCPSocket *sock,
                void *msgbuf,
                size_t buflen,
                size_t flags);

/**
 * Receives incoming traffic on tcp connections
 */
int tcp_recv(struct TCPSocket *sock,
                void *msgbuf,
                size_t buflen,
                size_t flags);

/**
 * Closes a tcp socket when you're done with it
 * @return non-zero on failure
 */
int tcp_free(TCPSocket* sock);
