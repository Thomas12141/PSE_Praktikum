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
# Pew pew!
cannon.pewpew()
