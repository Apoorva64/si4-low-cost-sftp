The SecTrans application, as described, has several potential security vulnerabilities that need to be addressed. Here
are some recommended security countermeasures:

1. **Secure Communication**: Since the communication library provided by Macrohard Corporation does not implement any
   security mechanism, it's crucial to ensure that the data transferred between the client and the server is encrypted.
   This can be achieved by implementing SSL/TLS for secure communication.

#TODO: Refactor this section
GPT :
From the provided code excerpts, it appears that the project has implemented some form of secure communication. The
Server.cpp file includes OpenSSL.h and OpenSSL_Utils.h, which suggests that OpenSSL is being used for secure
communication. OpenSSL is a robust, full-featured open-source toolkit that implements the Secure Sockets Layer (SSL) and
Transport Layer Security (TLS) protocols. Moreover, the sslHandshake function in the Server class indicates that an SSL
handshake is being performed. An SSL handshake is a process that establishes a secure connection between a client and a
server.

2. **Authentication**: Implement a strong authentication mechanism to verify the identity of the users before they can
   upload, download, or list files. This could be done using JWT tokens or OAuth.

3. **Authorization**: Implement an authorization mechanism to ensure that users can only access their own files. This
   could be done by associating each file with the user who uploaded it and checking this association whenever a file is
   accessed.

4. **Input Validation**: Validate all inputs to the application to prevent attacks such as SQL injection, command
   injection, etc. This includes not only user inputs but also filenames, file contents, etc.

5. **Error Handling**: Implement proper error handling to prevent information leakage through error messages. This
   includes not revealing any system information in error messages shown to the user.

6. **Logging and Monitoring**: Implement logging and monitoring to detect any suspicious activities in the system. This
   includes logging all access to files and monitoring for multiple failed login attempts, unusual file access patterns,
   etc.

7. **Secure File Storage**: Ensure that the files are stored securely on the server. This includes encrypting the files
   before storing them and ensuring that the server is secure and up-to-date.

8. **Secure Coding Practices**: Follow secure coding practices to prevent common vulnerabilities. This includes
   practices such as not using dangerous functions, checking array bounds, etc.

9. **Security Testing**: Perform security testing on the application to identify and fix any security vulnerabilities.
   This includes fuzzing, penetration testing, etc.

These countermeasures should be documented in the security architecture of the application and should be considered
throughout the development lifecycle of the application.