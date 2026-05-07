имя для базы данных warehouse.db рядом с sql запросами

sqlite3 warehouse.db < scheme.sql
sqlite3 warehouse.db < seed.sql

нужные либы sqlite3

сборка

cmake -S . -B build
cmake --build build
cd build ~из корня проекта
