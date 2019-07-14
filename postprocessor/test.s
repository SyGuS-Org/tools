(define-fun f ((name String)) String
    (str.++ (str.++ (str.++ (str.++ (str.++ (str.++ " (" (str.substr name 0 3)) " )") " ") (str.at name 4)) (str.substr name 5 5)) (str.at name (+ 5 5)))
)