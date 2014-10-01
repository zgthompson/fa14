(define reciprocal 
  (lambda (x)
    (if (= x 0)
      "oops!"
      (/ 1 x))))


(define square
 (lambda (x)
  (* x x)))


(define squareOfReciprocal
 (lambda (x)
  (square (reciprocal x))))


(define reciprocalOfSquare
 (lambda (x)
  (reciprocal (square x))))


(define doubleReciprocalOfSquare
 (lambda (x)
  (* 2 (reciprocalOfSquare x))))


(define halfSquareOfReciprocal
 (lambda (x)
  (/ (squareOfReciprocal x) 2)))


(define insert-first
 (lambda (lst val)
  (if (list? lst)
   (cons val lst)
   "First argument must be a list")))


(define remove-first
 (lambda (lst)
  (cond
   ((not (list? lst)) "First argument must be a list")
   ((null? lst) "First argument must not be an empty list")
   (else (cdr lst)))))


(define list-copy
 (lambda (lst)
  (cond
   ((not (list? lst)) "First argument must be a list")
   ((null? lst) lst)
   (else (cons (car lst) (list-copy(cdr lst)))))))


(define odd-copy
 (lambda (lst)
  (cond
   ((not (list? lst)) "First argument must be a list")
   ((or (null? lst) (null? (cdr lst))) lst)
   (else (cons (car lst) (odd-copy (cddr lst)))))))

