<?php
header('Content-Type: text/plain');
date_default_timezone_set('UTC');
printf("SERVER['REQUEST_TIME']: %s\n", date("r", $_SERVER['REQUEST_TIME']));
printf("time()                : %s\n", date("r", time()));
printf("microtime()           : %s\n", date("r", microtime(TRUE)));
printf("date('r')             : %s\n", date("r"));
printf("gettimeofday          : %s\n", date("r", gettimeofday(TRUE)));
