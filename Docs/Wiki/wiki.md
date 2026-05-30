# MathSolver Wiki

## Architecture
Client-Server application. Server: C++/Qt, TCP port 33333. Client: Qt Widgets.

## Protocol
Commands: auth, reg, verify, forgot, reset, calc_table, check
Separator: &

## Database
SQLite, file mydb.sqlite. Tables: users, stats.

## Build
Server: Qt Creator, Release mode
Client: build.bat or Qt Creator

## Dependencies
Qt6, OpenSSL (for email), GCC (MinGW64)
