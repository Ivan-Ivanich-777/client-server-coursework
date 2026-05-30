bool ServerCore::sendEmail(const QString& to, const QString& subject, const QString& body) {
    QSslSocket socket;
    qDebug() << "[MAIL] Connecting to smtp.gmail.com:587...";
    socket.connectToHost("smtp.gmail.com", 587);
    if (!socket.waitForConnected(5000)) { qDebug() << "[MAIL] Connection failed"; return false; }
    socket.waitForReadyRead(3000); socket.readAll();
    
    socket.write("EHLO localhost\r\n"); socket.waitForBytesWritten(1000);
    socket.waitForReadyRead(3000); socket.readAll();
    
    socket.write("STARTTLS\r\n"); socket.waitForBytesWritten(1000);
    socket.waitForReadyRead(3000); socket.readAll();
    
    socket.startClientEncryption();
    if (!socket.waitForEncrypted(5000)) { qDebug() << "[MAIL] Encryption failed"; return false; }
    
    socket.write("EHLO localhost\r\n"); socket.waitForBytesWritten(1000);
    socket.waitForReadyRead(3000); socket.readAll();
    
    socket.write("AUTH LOGIN\r\n"); socket.waitForBytesWritten(1000);
    socket.waitForReadyRead(3000); socket.readAll();
    
    socket.write(QByteArray("mthsolver@gmail.com").toBase64() + "\r\n"); socket.waitForBytesWritten(1000);
    socket.waitForReadyRead(3000); socket.readAll();
    
    socket.write(QByteArray("ylop wnvq wmoi tocq").toBase64() + "\r\n"); socket.waitForBytesWritten(1000);
    socket.waitForReadyRead(3000); socket.readAll();
    
    socket.write("MAIL FROM:<mthsolver@gmail.com>\r\n"); socket.waitForBytesWritten(1000);
    socket.waitForReadyRead(3000);
    socket.write("RCPT TO:<" + to.toUtf8() + ">\r\n"); socket.waitForBytesWritten(1000);
    socket.waitForReadyRead(3000);
    socket.write("DATA\r\n"); socket.waitForBytesWritten(1000);
    socket.waitForReadyRead(3000);
    socket.write("From: MathSolver <mthsolver@gmail.com>\r\nTo: " + to + "\r\nSubject: " + subject + "\r\n\r\n" + body + "\r\n.\r\n");
    socket.waitForBytesWritten(3000);
    socket.waitForReadyRead(5000);
    socket.readAll();
    
    socket.write("QUIT\r\n"); socket.waitForBytesWritten(1000);
    socket.close();
    qDebug() << "[MAIL] Sent successfully to:" << to;
    return true;
}
