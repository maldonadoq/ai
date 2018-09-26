(defglobal
    ?*number* = 40)

(defrule game
    =>
    (assert (tread)))

(defrule tread
    ?h <- (tread)
    =>
    (retract ?h)
    (printout t "  Write a number: ")
    (assert (number (read))))

(defrule bajo
    ?h <- (number ?n&:(< ?n ?*number*))
    =>
    (retract ?h)
    (printout t "    The number is less" crlf)
    (assert (tread)))

(defrule alto
    ?h <- (number ?n&:(> ?n ?*number*))
    =>
    (retract ?h)
    (printout t "    The number is greater" crlf)
    (assert (tread)))

(defrule exacto
    ?h <- (number ?n&:(= ?n ?*number*))
    =>
    (retract ?h)
    (printout t "    You guessed" crlf))