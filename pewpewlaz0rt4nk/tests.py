#!/usr/bin/python
# -*- coding: utf-8 -*-
#
# This script makes a HTTP request to a specific server and compares
# responses with user-defined expected responses.
from __future__ import unicode_literals

import sys

from pewpewlaz0rt4nk import (
    Beam,
    Laz0rCannon,
)

__version__ = '2.0.0'

# Set host and port.
# Read the target IP address and port from the command line arguments.
# If there are no command line arguments, use the following default values.
host = '127.0.0.1'
port = 31337

# Overwrite host and port (if specified)
if len(sys.argv) == 3:
    _, host, port = sys.argv
    port = int(port)

# Initialise Laz0rCannon
cannon = Laz0rCannon(host=host, port=port)

# Laz0r Beams (Tests to fire against the server).

cannon += Beam(
    description='Groß- und Kleinbuchstaben abwechselnd in Host-Header',
    request='GET / HTTP/1.1\r\nhOsT: {host}:{port}\r\n\r\n',
    response=['HTTP/1.1 200']
)
cannon += Beam(
    description='Ungültiges Http-Verb "POST"',
    request='POST /index.html HTTP/1.1\r\nHost: {host}:{port}\r\n\r\n',
    response=['HTTP/1.1 501']
)
cannon += Beam(
    description='Http-Verb in Kleinbuchstaben (get)',
    request='get /index.html HTTP/1.1\r\nHost: {host}:{port}\r\n\r\n',
    response=['HTTP/1.1 501']
)
cannon += Beam(
    description='Keine Ressource angegeben',
    request='GET  HTTP/1.1\r\nHost: {host}:{port}\r\n\r\n',
    response=['HTTP/1.1 400']
)
cannon += Beam(
    description='Keine Http-Version angegeben',
    request='GET /\r\nHost: {host}:{port}\r\n\r\n',
    response=['HTTP/1.1 400']
)
cannon += Beam(
    description='Falsche HTTP-Version -> HTTP/1.2',
    request='GET / HTTP/1.2\r\nHost: {host}:{port}\r\n\r\n',
    response=['HTTP/1.1 505']
)
cannon += Beam(
    description='Falsche HTTP-Version -> HTTP/1.268767',
    request='GET / HTTP/1.268767\r\nHost: {host}:{port}\r\n\r\n',
    response=['HTTP/1.1 505']
)
cannon += Beam(
    description='Falsche HTTP-Version -> HTTP/1453.00012933',
    request='GET / HTTP/1453.00012933\r\nHost: {host}:{port}\r\n\r\n',
    response=['HTTP/1.1 505']
)
cannon += Beam(
    description='HTTP-Version falsches Format -> HTTP/.1',
    request='GET / HTTP/.1\r\nHost: {host}:{port}\r\n\r\n',
    response=['HTTP/1.1 505']
)
cannon += Beam(
    description='HTTP-Version falsches Format -> HTT',
    request='GET / HTT\r\nHost: {host}:{port}\r\n\r\n',
    response=['HTTP/1.1 505']
)
cannon += Beam(
    description='HTTP-Version falsches Format -> /',
    request='GET / /\r\nHost: {host}:{port}\r\n\r\n',
    response=['HTTP/1.1 505']
)

cannon += Beam(
    description='HTTP-Version falsches Format -> HTTP/1.',
    request='GET / HTTP/1.\r\nHost: {host}:{port}\r\n\r\n',
    response=['HTTP/1.1 505']
)
cannon += Beam(
    description='GET falsch geschrieben -> GE',
    request='GE / HTTP/1.1\r\nHost: {host}:{port}\r\n\r\n',
    response=['HTTP/1.1 501']
)
cannon += Beam(
    description='Illegaler Zugriff -> /../../',
    request='GET /../../ HTTP/1.1\r\nHost: {host}:{port}\r\n\r\n',
    response=['HTTP/1.1 403']
)
cannon += Beam(
    description='Zugriff auf Datei außerhalb -> /../httplib.h',
    request='GET /../httplib.h HTTP/1.1\r\nHost: {host}:{port}\r\n\r\n',
    response=['HTTP/1.1 403']
)
cannon += Beam(
    description='Leerzeichen',
    request='GET     /     HTTP/1.1\r\nHost: {host}:{port}\r\n\r\n',
    response=['HTTP/1.1 400']
)
cannon += Beam(
    description='Langer Index',
    request='GET /dddddddddddddddddddddddddddddddddddddddddddddddddddddddd HTTP/1.1\r\nHost: {host}:{port}\r\n\r\n',
    response=['HTTP/1.1 404']
)
cannon += Beam(
    description='Längerer Index',
    request='GET /ddddddddddddddflksjdfkdsfjdsfkldsjfkdsljflkdsjflkdjlskjlkdjdddddddddddddddddddddddddddddddddddddddddd HTTP/1.1\r\nHost: {host}:{port}\r\n\r\n',
    response=['HTTP/1.1 404']
)
cannon += Beam(
    description='Leerer Request',
    request='Host: {host}:{port}\r\n\r\n',
    response=['HTTP/1.1 400']
)
cannon += Beam(
    description='Kein GET',
    request=' / HTTP/1.1\r\nHost: {host}:{port}\r\n\r\n',
    response=['HTTP/1.1 400']
)
cannon += Beam(
    description='Keine HTTP-Version',
    request='GET / \r\nHost: {host}:{port}\r\n\r\n',
    response=['HTTP/1.1 400']
)
cannon += Beam(
    description='Nur Leerzeichen',
    request='           Host: {host}:{port}\r\n\r\n',
    response=['HTTP/1.1 400']
)
cannon += Beam(
    description='Keine Ressource',
    request='GET  HTTP/1.1\r\nHost: {host}:{port}\r\n\r\n',
    response=['HTTP/1.1 400']
)
cannon += Beam(
    description='Nur Path /',
    request='HTTP/1.1\r\nHost: {host}:{port}\r\n\r\n',
    response=['HTTP/1.1 400']
)
cannon += Beam(
    description='GET GET GET',
    request='GET GET GET\r\nHost: {host}:{port}\r\n\r\n',
    response=['HTTP/1.1 505']
)
cannon += Beam(
    description='HTTP/1.1 HTTP/1.1 HTTP/1.1',
    request='HTTP/1.1 \HTTP/1.1 HTTP/1.1\r\nHost: {host}:{port}\r\n\r\n',
    response=['HTTP/1.1 404']
)
cannon += Beam(
    description='HTTP/1.1 HTTP/1.1 HTTP/1.1 mit zusätzlichen Zeilenumbrüchen',
    request='HTTP/1.1 \HTTP/1.1 HTTP/1.1\r\nHost: {host}:{port}\r\n\r\n',
    response=['HTTP/1.1 404']
)
cannon += Beam(
    description='Request durcheinander',
    request='GET / / / /../ HTTP/1.1 get \r\nHost: {host}:{port}\r\n\r\n',
    response=['HTTP/1.1 505']
)
cannon += Beam(
    description='Langer Index mit unnötigen Zeilenvorschüben und -umbrüchen',
    request='GET /ddddddddddddddddddddddddddddddffffffffffffffff\r\ndddddddd\rddddddddfksf\njdskjflksjfdkfksdjfkjdslkfjdsfjkfdskfjlsddddddddddd HTTP/1.1\r\nHost: {host}:{port}\r\n\r\n',
    response=['HTTP/1.1 400']
)
cannon += Beam(
    description='GEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEET',
    request='GEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEET / HTTP/1.1\r\nHost: {host}:{port}\r\n\r\n',
    response=['HTTP/1.1 501']
)
cannon += Beam(
    description='YEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEET',
    request='YEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEET / HTTP/1.1\r\nHost: {host}:{port}\r\n\r\n',
    response=['HTTP/1.1 501']
)
cannon += Beam(
    description='YEEEEEEEEEEEEEEEEEEE + EEEEEEEEEEEEEEEEEEEEEEEEEEEEET',
    request='YEEEEEEEEEEEEEEEEEEEEEEE' + 'EEEEEEEEEEEEEEEEEEEEEEEEET / HTTP/1.1\r\nHost: {host}:{port}\r\n\r\n',
    response=['HTTP/1.1 501']
)
cannon += Beam(
    description='42424242424242',
    request='42424242424242\r\nHost: {host}:{port}\r\n\r\n',
    response=['HTTP/1.1 400']
)
cannon += Beam(
    description='intern',
    request='GET / HTTP/1.1\r\nHost: intern:{port}\r\n\r\n',
    response=['HTTP/1.1 401']
)
cannon += Beam(
    description='extern',
    request='GET / HTTP/1.1\r\nHost: extern:{port}\r\n\r\n',
    response=['HTTP/1.1 200']
)
cannon += Beam(
    description='intern 404',
    request='GET /ttt.txt HTTP/1.1\r\nHost: intern:{port}\r\n\r\n',
    response=['HTTP/1.1 401']
)
cannon += Beam(
    description='extern 404',
    request='GET /ttt.txt HTTP/1.1\r\nHost: extern:{port}\r\n\r\n',
    response=['HTTP/1.1 404']
)
cannon += Beam(
    description='falsche HTTP-Version kaputt',
    request='GET / HTTP/1.1getgetget\r\nHost: {host}:{port}\r\n\r\n',
    response=['HTTP/1.1 505']
)
cannon += Beam(
    description='Durcheinander..... aber anders',
    request='GET / / /////////////// / /../ HTTP/1.1 get\r\n\r\nHost: {host}:{port}\r\n\r\n',
    response=['HTTP/1.1 505']
)
cannon += Beam(
    description='Durcheinander..... aber anders UND außer Haus',
    request='GET / / /////////////// / /../ HTTP/1.1 get\r\n\r\nHost: extern:{port}\r\n\r\n',
    response=['HTTP/1.1 505']
)
cannon += Beam(
    description='Durcheinander..... aber anders UND im Haus',
    request='GET / / /////////////// / /../ HTTP/1.1 get\r\n\r\nHost: intern:{port}\r\n\r\n',
    response=['HTTP/1.1 505']
)
cannon += Beam(
    description='PUT',
    request='PUT / / /////////////// / /../ HTTP/1.1 get\r\n\r\nHost: intern:{port}\r\n\r\n',
    response=['HTTP/1.1 505']
)
cannon += Beam(
    description='Anderes PUT',
    request='PUT / HTTP/1.1\r\n\r\nHost: intern:{port}\r\n\r\n',
    response=['HTTP/1.1 401']
)
cannon += Beam(
    description='Tabulatoren',
    request='GET           / HTTP/1.1\r\nHost: {host}:{port}\r\n\r\n',
    response=['HTTP/1.1 400']
)
cannon += Beam(
    description='PUT default',
    request='PUT / HTTP/1.1\r\n\r\nHost: {host}:{port}\r\n\r\n',
    response=['HTTP/1.1 501']
)
cannon += Beam(
    description='intern random',
    request='GET sfelkjgflkfd\r\n\r\nHost: intern:{port}\r\n\r\n',
    response=['HTTP/1.1 400']
)
cannon += Beam(
    description='HEAD default',
    request='HEAD / HTTP/1.1\r\n\r\nHost: {host}:{port}\r\n\r\n',
    response=['HTTP/1.1 501']
)
cannon += Beam(
    description='OPTIONS default',
    request='OPTIONS / HTTP/1.1\r\n\r\nHost: {host}:{port}\r\n\r\n',
    response=['HTTP/1.1 501']
)
cannon += Beam(
    description='TRACE default',
    request='TRACE / HTTP/1.1\r\n\r\nHost: {host}:{port}\r\n\r\n',
    response=['HTTP/1.1 501']
)
cannon += Beam(
    description='CONNECT default',
    request='CONNECT / HTTP/1.1\r\n\r\nHost: {host}:{port}\r\n\r\n',
    response=['HTTP/1.1 501']
)
cannon += Beam(
    description='Langer Index mit unnötigen Zeilenvorschüben und -umbrüchen',
    request='GET /dddddddddddddddddddddddddddfksfjdskjflksjfdkfksdjskfjlsdddddddddddddddddddddddddddddddddddddddddffffffffffffffffddddddddddddddddfksfjdskjflksjfdkfksdjfkjdslkhdfssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssfjdsfjkfdskfjlsdddddddddddddddddddddddddddddddddddddddddffffffffffffffffddddddddddddddddfksfjdskjflksjfdkfksdjfkjdslkhdfssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssfjdsfjkfdskfjlsdddddddddddddddddddddddddddddddddddddddddffffffffffffffffddddddddddddddddfksfjdskjflksjfdkfksdjfkjdslkhdfssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssfjdsfjkfdskfjlsdddddddddddddddddddddddddddddddddddddddddffffffffffffffffddddddddddddddddfksfjdskjflksjfdkfksdjfkjdslkhdfssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssfjdsfjkfdskfjlsdddddddddddddddddddddddddddddddddddddddddffffffffffffffffddddddddddddddddfksfjdskjflksjfdkfksdjfkjdslkhdfssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssfjdsfjkfdskfjlsdddddddddddddddddddddddddddddddddddddddddffffffffffffffffddddddddddddddddfksfjdskjflksjfdkfksdjfkjdslkhdfssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssfjdsfjkfdskfjlsdddddddddddddddddddddddddddddddddddddddddffffffffffffffffddddddddddddddddfksfjdskjflksjfdkfksdjfkjdslkhdfssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssfjdsfjkfdskfjlsdddddddddddddddddddddddddddddddddddddddddffffffffffffffffddddddddddddddddfksfjdskjflksjfdkfksdjfkjdslkhdfssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssfjdsfjkfdskfjlsdddddddddddddddddddddddddddddddddddddddddffffffffffffffffddddddddddddddddfksfjdskjflksjfdkfksdjfkjdslkhdfssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssfjdsfjkfdskfjlsdddddddddddddddddddddddddddddddddddddddddffffffffffffffffddddddddddddddddfksfjdskjflksjfdkfksdjfkjdslkhdfssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssfjdsfjkfdskfjlsdddddddddddddddddddddddddddddddddddddddddffffffffffffffffddddddddddddddddfksfjdskjflksjfdkfksdjfkjdslkhdfssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssfjdsfjkfdskfjlsdddddddddddddddddddddddddddddddddddddddddffffffffffffffffddddddddddddddddfksfjdskjflksjfdkfksdjfkjdslkhdfssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssfjdsfjkfdskfjlsdddddddddddddddddddddddddddddddddddddddddffffffffffffffffddddddddddddddddfksfjdskjflksjfdkfksdjfkjdslkhdfssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssfjdsfjkfdskfjlsdddddddddddddddddddddddddddddddddddddddddffffffffffffffffddddddddddddddddfksfjdskjflksjfdkfksdjfkjdslkhdfssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssfjdsfjkfdskfjlsddddddddddd HTTP/1.1\r\nHost: {host}:{port}\r\n\r\n',
    response=['HTTP/1.1 404']
)

cannon += Beam(
    description='Terminierungszeichen im Dateipfad',
    request='GET /images/tu%00x.jpg HTTP/1.1\r\nHost: {host}:{port}\r\n\r\n',
    response=['HTTP/1.1 404']
)
cannon += Beam(
    description='htpasswd auslesen',
    request='GET /../htpasswd HTTP/1.1\r\nHost: {host}:{port}\r\n\r\n',
    response=['HTTP/1.1 403']
)

############################################################################################
# Folgende Tests wurden uns freundlicherweise von der Gruppe PG4-1 zur Verfügung gestellt. #
############################################################################################

cannon += Beam(
    description='Host in Kleinbuchstaben (host)',
    request='GET / HTTP/1.1\r\nhost: {host}:{port}\r\n\r\n',
    response=['HTTP/1.1 200']
)

cannon += Beam(
    description='Kein Host-Header HTTP/1.0',
    request='GET / HTTP/1.0\r\n\r\n',
    response=['HTTP/1.1 505']
)

cannon += Beam(
    description='Debug-Seite',
    request='GET /debug HTTP/1.1\r\nHost: {host}:{port}\r\n\r\n',
    response=['HTTP/1.1 200']
)

cannon += Beam(
    description='Host "intern"',
    request='GET / HTTP/1.1\r\nHost: intern\r\n\r\n',
    response=['HTTP/1.1 401', 'Content-Type: ', 'Content-Length: ', 'WWW-Authenticate: Basic realm="Login"']
)

cannon += Beam(
    description='Host "intern" mit Leerzeichen am Ende',
    request='GET / HTTP/1.1\r\nHost: intern \r\n\r\n',
    response=['HTTP/1.1 401', 'Content-Type: ', 'Content-Length: ', 'WWW-Authenticate: Basic realm="Login"']
)

cannon += Beam(
    description='Host "extern"',
    request='GET / HTTP/1.1\r\nHost: extern\r\n\r\n',
    response=['HTTP/1.1 200 OK',  'Content-Type: text/html', 'Content-Length: 246', '',
              '<html>',
              '        <head>',
              '                <title>PSE Extern</title>',
              '                <script language="JavaScript" src="js/javascript.js" ></script>',
              '        </head>',
              '        <body>'
              ]
)

cannon += Beam(
    description='Host "extern" mit Port',
    request='GET / HTTP/1.1\r\nHost: extern:8080\r\n\r\n',
    response=['HTTP/1.1 200 OK', 'Content-Type: text/html', 'Content-Length: 246', '',
              '<html>',
              '        <head>',
              '                <title>PSE Extern</title>',
              '                <script language="JavaScript" src="js/javascript.js" ></script>',
              '        </head>',
              '        <body>'
              ]
)

cannon += Beam(
    description='HTTP/1.0',
    request='GET / HTTP/1.0\r\nHost: extern\r\n\r\n',
    response=['HTTP/1.1 505']
)

cannon += Beam(
    description='Ungültige HTTP-Version',
    request='GET / HTTP/1.2\r\nHost: extern\r\n\r\n',
    response=['HTTP/1.1 505']
)

cannon += Beam(
    description='Keine Ressource und keine HTTP-Version angegeben',
    request='GET \r\n\r\n',
    response=['HTTP/1.1 400']
)

cannon += Beam(
    description='default/index.html: passende Content-Length und Content-Type',
    request='GET /index.html HTTP/1.1\r\nHost: {host}:{port}\r\n\r\n',
    response=['HTTP/1.1 200', 'Content-Type: text/html', 'Content-Length: 5900']
)

cannon += Beam(
    description='default/images/TUX1.PNG: passende Content-Length und Content-Type',
    request='GET /images/TUX1.PNG HTTP/1.1\r\nHost: {host}:{port}\r\n\r\n',
    response=['HTTP/1.1 200', 'Content-Type: image/png', 'Content-Length: 39594']
)

cannon += Beam(
    description='default/images/tux.jpg: passende Content-Length und Content-Type',
    request='GET /images/tux.jpg HTTP/1.1\r\nHost: {host}:{port}\r\n\r\n',
    response=['HTTP/1.1 200', 'Content-Type: image/jpeg', 'Content-Length: 9883']
)

cannon += Beam(
    description='default/images/tux.png: passende Content-Length und Content-Type',
    request='GET /images/tux.png HTTP/1.1\r\nHost: {host}:{port}\r\n\r\n',
    response=['HTTP/1.1 200', 'Content-Type: image/png', 'Content-Length: 39594']
)

cannon += Beam(
    description='default/js/javascript.js: passende Content-Length und Content-Type',
    request='GET /js/javascript.js HTTP/1.1\r\nHost: {host}:{port}\r\n\r\n',
    response=['HTTP/1.1 200', 'Content-Type: text/plain', 'Content-Length: 74']
)

cannon += Beam(
    description='default/images/tux: passende Content-Length',
    request='GET /images/tux HTTP/1.1\r\nHost: {host}:{port}\r\n\r\n',
    response=['HTTP/1.1 200', '', 'Content-Length: 39594']
)

cannon += Beam(
    description='default/images/ein leerzeichen.png: passende Content-Length und Content-Type',
    request='GET /images/ein%20leerzeichen.png HTTP/1.1\r\nHost: {host}:{port}\r\n\r\n',
    response=['HTTP/1.1 200', 'Content-Type: image/png', 'Content-Length: 39594']
)

cannon += Beam(
    description='default/images'
                '/aaaaaaaaaaaaaaaaaaa[...].png: 404 Error',
    request='GET '
            '/images/aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa.png HTTP/1.1\r\nHost: {host}:{port}\r\n\r\n',
    response=['HTTP/1.1 404']
)

cannon += Beam(
    description='implizites index.html',
    request='GET / HTTP/1.1\r\nHost: {host}:{port}\r\n\r\n',
    response=['HTTP/1.1 200', 'Content-Type: text/html', 'Content-Length: 5900']
)

cannon += Beam(
    description='Pfad außerhalb von DocumentRoot',
    request='GET /../../../../../etc/passwd HTTP/1.1\r\nHost: {host}:{port}\r\n\r\n',
    response=['HTTP/1.1 403']
)

cannon += Beam(
    description='Zugriff auf Datei in "intern" mit Host "default"',
    request='GET /../intern/index.html HTTP/1.1\r\nHost: {host}:{port}\r\n\r\n',
    response=['HTTP/1.1 403']
)

cannon += Beam(
    description='Methode: POST',
    request='POST / HTTP/1.1\r\nHost: {host}:{port}\r\n\r\n',
    response=['HTTP/1.1 501']
)

cannon += Beam(
    description='Methode: HEAD',
    request='HEAD / HTTP/1.1\r\nHost: {host}:{port}\r\n\r\n',
    response=['HTTP/1.1 501']
)

cannon += Beam(
    description='Methode: PUT',
    request='PUT / HTTP/1.1\r\nHost: {host}:{port}\r\n\r\n',
    response=['HTTP/1.1 501']
)

cannon += Beam(
    description='Methode: DELETE',
    request='DELETE / HTTP/1.1\r\nHost: {host}:{port}\r\n\r\n',
    response=['HTTP/1.1 501']
)

cannon += Beam(
    description='Methode: CONNECT',
    request='CONNECT / HTTP/1.1\r\nHost: {host}:{port}\r\n\r\n',
    response=['HTTP/1.1 501']
)

cannon += Beam(
    description='Methode: OPTIONS',
    request='OPTIONS / HTTP/1.1\r\nHost: {host}:{port}\r\n\r\n',
    response=['HTTP/1.1 501']
)

cannon += Beam(
    description='Methode: TRACE',
    request='TRACE / HTTP/1.1\r\nHost: {host}:{port}\r\n\r\n',
    response=['HTTP/1.1 501']
)

cannon += Beam(
    description='Methode: PATCH',
    request='PATCH / HTTP/1.1\r\nHost: {host}:{port}\r\n\r\n',
    response=['HTTP/1.1 501']
)

cannon += Beam(
    description='Keine HTTP-Methode (HTTP/1.0)',
    request='/ HTTP/1.1\r\nHost: {host}:{port}\r\n\r\n',
    response=['HTTP/1.1 400']
)

cannon += Beam(
    description='Keine HTTP-Methode mit Leerzeichen (HTTP/1.0)',
    request=' / HTTP/1.1\r\nHost: {host}:{port}\r\n\r\n',
    response=['HTTP/1.1 400']
)

cannon += Beam(
    description='Keine HTTP-Methode (HTTP/1.1)',
    request='/ HTTP/1.1\r\nHost: {host}:{port}\r\n\r\n',
    response=['HTTP/1.1 400']
)

cannon += Beam(
    description='Keine HTTP-Methode mit Leerzeichen (HTTP/1.1)',
    request=' / HTTP/1.1\r\nHost: {host}:{port}\r\n\r\n',
    response=['HTTP/1.1 400']
)

cannon += Beam(
    description='Echo_Server /debug',
    request='GET /debug HTTP/1.1\r\nHost: {host}:{port}\r\n\r\n',
    response=['HTTP/1.1 200 OK']
)

cannon += Beam(
    description='Fehlendes 1. Space in der Request',
    request='GET/index.html HTTP/1.1\r\nHost: {host}:{port}\r\n\r\n',
    response=['HTTP/1.1 400 Bad Request']
)

cannon += Beam(
    description='Fehlende Spaces in der Request',
    request='GET/index.htmlHTTP/1.1\r\nHost: {host}:{port}\r\n\r\n',
    response=['HTTP/1.1 400 Bad Request']
)

cannon += Beam(
    description='Fehlender Absatz in der Request',
    request='GET /index.html HTTP/1.1Host: {host}:{port}\r\n\r\n',
    response=['HTTP/1.1 505']
)

cannon += Beam(
    description='Ressource beginnt nicht mit /',
    request='GET index.html HTTP/1.1\r\nHost: {host}:{port}\r\n\r\n',
    response=['HTTP/1.1 404']
)

cannon += Beam(
    description='Valide Methode POST, die nicht unterstützt wird',
    request='POST /index.html HTTP/1.1\r\nHost: {host}:{port}\r\n\r\n',
    response=['HTTP/1.1 501']
)

cannon += Beam(
    description='Valide Methode HEAD, die nicht unterstützt wird',
    request='HEAD /index.html HTTP/1.1\r\nHost: {host}:{port}\r\n\r\n',
    response=['HTTP/1.1 501']
)

cannon += Beam(
    description='Valide Methode PUT, die nicht unterstützt wird',
    request='PUT /index.html HTTP/1.1\r\nHost: {host}:{port}\r\n\r\n',
    response=['HTTP/1.1 501']
)

cannon += Beam(
    description='Valide Methode DELETE, die nicht unterstützt wird',
    request='DELETE /index.html HTTP/1.1\r\nHost: {host}:{port}\r\n\r\n',
    response=['HTTP/1.1 501']
)

cannon += Beam(
    description='Valide Methode CONNECT, die nicht unterstützt wird',
    request='CONNECT /index.html HTTP/1.1\r\nHost: {host}:{port}\r\n\r\n',
    response=['HTTP/1.1 501']
)

cannon += Beam(
    description='Valide Methode OPTIONS, die nicht unterstützt wird',
    request='OPTIONS /index.html HTTP/1.1\r\nHost: {host}:{port}\r\n\r\n',
    response=['HTTP/1.1 501']
)

cannon += Beam(
    description='Valide Methode TRACE, die nicht unterstützt wird',
    request='TRACE /index.html HTTP/1.1\r\nHost: {host}:{port}\r\n\r\n',
    response=['HTTP/1.1 501']
)

cannon += Beam(
    description='Valide Methode PATCH, die nicht unterstützt wird',
    request='PATCH /index.html HTTP/1.1\r\nHost: {host}:{port}\r\n\r\n',
    response=['HTTP/1.1 501']
)

cannon += Beam(
    description='Invalide Methode: LINK',
    request='LINK / HTTP/1.1\r\n\r\n',
    response=['HTTP/1.1 501']
)
cannon += Beam(
    description='Invalide Methode: UNLINK',
    request='LINK / HTTP/1.1\r\n\r\n',
    response=['HTTP/1.1 501']
)

cannon += Beam(
    description='Invalide Methode',
    request='INVALID_METHOD /index.html HTTP/1.1\r\nHost: {host}:{port}\r\n\r\n',
    response=['HTTP/1.1 501']
)

cannon += Beam(
    description='HTTP/2, valide aber nicht unterstützt',
    request='GET /index.html HTTP/2\r\n\r\n',
    response=['HTTP/1.1 505']
)

cannon += Beam(
    description='HTTP/3, valide aber nicht unterstützt',
    request='GET /index.html HTTP/3\r\n\r\n',
    response=['HTTP/1.1 505']
)

cannon += Beam(
    description='Valide Methode OPTIONS, die nicht unterstützt wird',
    request='OPTIONS /index.html HTTP/1.1\r\nHost: {host}:{port}\r\n\r\n',
    response=['HTTP/1.1 501']
)

cannon += Beam(
    description='HTTP/0.9, mit get kleingeschrieben',
    request='get /uiofuiozgf\r\n',
    response=['HTTP/1.1 400 Bad Request']
)

cannon += Beam(
    description='HTTP/0.9 aber mit POST',
    request='POST /\r\n',
    response=['HTTP/1.1 400 Bad Request']
)

cannon += Beam(
    description='HTTP/0.9 aber mit Leerzeichen zuviel',
    request='GET / \r\n',
    response=['HTTP/1.1 400']
)

cannon += Beam(
    description='Echo_Server /debug mit nicht standard Methode',
    request='Custom_GET_Method /debug HTTP/1.0\r\n\r\n',
    response=['HTTP/1.1 200 OK']
)

cannon += Beam(
    description='Keine Ressource angegeben',
    request='GET  HTTP/1.1\r\nHost: {host}:{port}\r\n\r\n',
    response=['HTTP/1.1 400']
)

cannon += Beam(
    description='Keine Http-Version angegeben',
    request='GET /\r\nHost: {host}:{port}\r\n\r\n',
    response=['HTTP/1.1 400']
)

cannon += Beam(
    description='ShortRequest ohne ressource',
    request='GET \r\n',
    response=['HTTP/1.1 400']
)

cannon += Beam(
    description='long Path"',
    request='GET /index.html/index.html/index.html/index.html/index.htmlv/index.htmlv/index.html/index.html/index.html/index.html/index.html HTTP/1.1\r\nHost: {host}\r\n\r\n',
    response=['HTTP/1.1 404']
)

cannon += Beam(
    description='Space before Method:Get"',
    request=' GET / HTTP/1.1\r\nHost: {host}:{port}\r\n\r\n',
    response=['HTTP/1.1 501']
)

cannon += Beam(
    description='Kein CRLF nach 0.9 Anfrage',
    request='GET /index.html',
    response=[
        'HTTP/1.1 400 Bad Request'
    ]
)

cannon += Beam(
    description='Nur GET',
    request='GET\r\n\r\n',
    response=[
        'HTTP/1.1 400 Bad Request'
    ]
)

cannon += Beam(
    description='Nur GET mit zwei Leerzeichen',
    request='GET  \r\n',
    response=[
        'HTTP/1.1 400 Bad Request'
    ]
)

cannon += Beam(
    description='Nur GET mit zwei Leerzeichen und NewLine',
    request='GET  \r\n\r\n',
    response=[
        'HTTP/1.1 400 Bad Request'
    ]
)

cannon += Beam(
    description='Nur GET mit Pfad, ohne CRLF',
    request='GET /',
    response=[
        'HTTP/1.1 400 Bad Request'
    ]
)

cannon += Beam(
    description='Anfrage ohne Header, keine leere Zeile',
    request='GET / HTTP/1.0\r\n',
    response=['HTTP/1.1 505']
)

cannon += Beam(
    description='URI "/n\\r\\ndex.html"',
    request='GET /n\r\ndex.html HTTP/1.1\r\nHost: {host}\r\n\r\n',
    response=['HTTP/1.1 400']
)

cannon += Beam(
    description='Kein CR',
    request='GET /index.html HTTP/1.0\n\r\n',
    response=['HTTP/1.1 400 Bad Request']
)

cannon += Beam(
    description='Nicht ASCII Alphabet',
    request='GET /бел HTTP/1.0\r\n\r\n',
    response=['HTTP/1.1 505']
)

cannon += Beam(
    description='Keine ASCII Zeichen',
    request='GET /ÜÖ.html HTTP/1.1\r\n\r\n',
    response=['HTTP/1.1 404']
)

cannon += Beam(
    description='Datei Punkt URL Encoded',
    request='GET /images/tux%2Epng HTTP/1.1\r\n\r\n',
    response=['HTTP/1.1 200']
)

cannon += Beam(
    description='Lange Anfrage',
    request='GET / HTTP/1.1\r\n' + 'Connection: close' * 100 + '\r\n\r\n',
    response=['HTTP/1.1 200 OK'],
)

cannon += Beam(
    description='Bad URI "/in\\0dex.html"',
    request='GET /in\0dex.html HTTP/1.1\r\nHost: {host}\r\n\r\n',
    response=['HTTP/1.1 404']
)

cannon += Beam(
    description='Bad URI "/in\\xffdex.html"',
    request='GET /in\xffdex.html HTTP/1.1\r\nHost: {host}\r\n\r\n',
    response=['HTTP/1.1 404']
)

cannon += Beam(
    description='Bad URI "/in\\tdex.html"',
    request='GET /in\tdex.html HTTP/1.1\r\nHost: {host}\r\n\r\n',
    response=['HTTP/1.1 404']
)

cannon += Beam(
    description='Bad URI chars "/inäÖÜdex.html"',
    request='GET /inäÖÜdex.html HTTP/1.1\r\nHost: {host}\r\n\r\n',
    response=['HTTP/1.1 404']
)

cannon += Beam(
    description='NULL before Header',
    request='GET /index.html HTTP/1.1\r\0\n\0Host: {host}\r\n\r\n',
    response=['HTTP/1.1 200']
)

cannon += Beam(
    description='Request is double CRLF',
    request='\r\n\r\n',
    response=['HTTP/1.1 400']
)

cannon += Beam(
    description='Access with %20',
    request='GET /images/ein%20leerzeichen.png HTTP/1.1\r\nHost: {host}\r\n\r\n',
    response=['HTTP/1.1 200']
)

cannon += Beam(
    description='GET with Entity-Body and without Content-Length',
    request='GET /index.html HTTP/1.1\r\nHost: {host}\r\n\r\nTESTTESTTEST',
    response=['HTTP/1.1 200']
)

cannon += Beam(
    description='GET with Entity-Body and Content-Length: 10',
    request='GET /index.html HTTP/1.1\r\nContent-Length: 10\r\nHost: {host}\r\n\r\nTESTTESTTE',
    response=['HTTP/1.1 200']
)

cannon += Beam(
    description='Groß- und Kleinbuchstaben abwechselnd in Host-Header',
    request='GET /debug HTTP/1.1\r\nhOsT: {host}:{port}\r\n\r\n',
    response=['HTTP/1.1 200']
)

cannon += Beam(
    description='host header is intern',
    request='GET /index.html HTTP/1.1\r\nHost:  intern    \r\n\r\n',
    response=['HTTP/1.1 401']
)

# Pew pew!
cannon.pewpew()
