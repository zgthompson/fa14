; recursively reverse the input list (and sublists of that list)
(define all-reverse
 (lambda (lst)
  (if (or (not (list? lst)) (null? lst))
   lst
   (append (all-reverse (cdr lst)) (list (all-reverse (car lst)))))))

; create a new list containing all odd indexed elements
(define odd-copy
 (lambda (lst)
  (cond
   ((not (list? lst)) "First argument must be a list")
   ((or (null? lst) (null? (cdr lst))) lst)
   (else (cons (car lst) (odd-copy (cddr lst)))))))

; create a new list containing all even indexed elements
(define even-copy
 (lambda (lst)
  (cond
   ((not (list? lst)) "First argument must be a list")
   ((or (null? lst) (null? (cdr lst))) '())
   (else (cons (car (cdr lst)) (even-copy (cddr lst)))))))

; returns a copy of the list with only integers
(define integer-copy
 (lambda (lst)
  (cond 
   ((null? lst) lst)
   ((integer? (car lst)) (cons (car lst) (integer-copy (cdr lst))))
   (else (integer-copy (cdr lst))))))


; takes in two sorted lists and merges them into one sorted list
(define merge
 (lambda (lst1 lst2)
  (cond
   ((and (null? lst1) (null? lst2)) '())
   ((null? lst1) lst2)
   ((null? lst2) lst1)
   ((> (car lst1) (car lst2)) (cons (car lst1) (merge (cdr lst1) lst2)))
   (else (cons (car lst2) (merge lst1 (cdr lst2)))))))

; sorts a list of integers
(define mergesort-helper
 (lambda (lst)
  (if (or (null? lst) (null? (cdr lst))) 
   lst
   (merge (mergesort-helper (odd-copy lst)) (mergesort-helper (even-copy lst))))))

(define mergesort
 (lambda (lst)
  (if (not (list? lst)) 
   "First argument must be a list"
   (mergesort-helper (integer-copy lst)))))

(define quicksort-helper
 (lambda (lst)
  (if (null? lst)
   lst
   (append (quicksort-helper (filter (lambda (x) (< x (car lst))) (cdr lst))) (cons (car lst) (quicksort-helper (filter (lambda (x) (or (= x (car lst)) (> x (car lst)))) (cdr lst))))))))

(define quicksort
 (lambda (lst)
  (if (not (list? lst)) 
   "First argument must be a list"
   (quicksort-helper (integer-copy lst)))))


