(begin (define make-point (lambda (x y) (set-property "object-name" "point" (list x y)))) (define make-line (lambda (x y) (set-property "object-name" "line" (list x y)))) (define make-text (lambda (x) (set-property "object-name" "text" (x)))))