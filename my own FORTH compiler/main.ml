(* SOUAIBY CHRISTINA 21102782
   SAMAHA ELIO 21105733 *)

(* *********** Question 1.a *********** *)

type op_Pile = DUP | DROP | SWAP | ROT
               
type op_Arith =  TIMES | DIV | PLUS | MINUS

type op_Comp = EQUALS | DIFF | LT | GT 
               
type op_Cond = IF | THEN | ELSE | ENDIF
                        
type element = OP_PILE of op_Pile | OP_ARITH of op_Arith | OP_COMP of op_Comp | OP_COND of op_Cond | CST_INT of int |  CST_BOOL of bool | CST_STRING of string | BEGIN_DEF | END_DEF(* TODO *)

(* *********** Question 1.b *********** *)

let to_string (x:element) : string = 
  match x with
  | OP_PILE p -> 
      (match p with 
       | DUP -> "DUP" 
       | DROP -> "DROP" 
       | SWAP -> "SWAP" 
       | ROT -> "ROT")
      
  | OP_ARITH a -> 
      (match a with
       | TIMES  -> "*"
       | DIV -> "/"
       | PLUS -> "+"
       | MINUS -> "-")
  | OP_COMP comp ->
      (match comp with 
       | EQUALS -> "="
       | DIFF -> "<>"
       | LT -> "<"
       | GT -> ">")
  |OP_COND cond ->
      (match cond with
       | IF -> "IF"
       | THEN -> "THEN"
       | ELSE -> "ELSE"
       | ENDIF -> "ENDIF" )
  | CST_INT i -> string_of_int i
  | CST_BOOL b -> if b then "TRUE" else "FALSE" 
  | CST_STRING s -> s
  | BEGIN_DEF -> ":"
  | END_DEF -> ";"

let of_string (s:string) : element = match s with
  |"DUP" -> OP_PILE DUP
  |"DROP" -> OP_PILE DROP
  |"SWAP" -> OP_PILE SWAP
  |"ROT" -> OP_PILE ROT
  |"*" -> OP_ARITH TIMES
  |"/" -> OP_ARITH DIV
  |"+" -> OP_ARITH PLUS
  |"-" -> OP_ARITH MINUS
  |"=" -> OP_COMP EQUALS
  |"<>" -> OP_COMP DIFF
  |"<" -> OP_COMP LT
  |">" -> OP_COMP GT
  |"IF" -> OP_COND IF
  |"THEN" -> OP_COND THEN
  |"ELSE" -> OP_COND ELSE
  |"ENDIF" -> OP_COND ENDIF
  |"TRUE" -> CST_BOOL true
  |"FALSE" -> CST_BOOL false
  |":" -> BEGIN_DEF
  |";" -> END_DEF
  |s -> match (int_of_string_opt s) with
    |None ->  CST_STRING s
    |Some a -> CST_INT a


(* *********** Question 1.c *********** *)

(** fonction utilitaire : 
    [split s] découpe le texte [s] en une suite de mot. 
*)
let split (s:string) : string list =
  (* traite les tabulations et les sauts de lignes comme des espaces *)
  let normalize_s = String.map (function '\n' | '\t' | '\r' -> ' ' | c -> c) s in
  let split_s = String.split_on_char ' ' normalize_s in
  (* ignore les espaces successifs *)
  List.filter ((<>) "") split_s ;;

assert (split "  \t  \n " = []) ;;
assert (split " A     \n B\t C  " = ["A";"B";"C"]) ;;

(** transforme un texte (représentant un programme ou une pile)
    en une suite de symboles du langage (e.g., "42" et "DUP") 
*)
let parse (s:string) : element list =
  List.map of_string (split s)

(** transforme un suite de symbole du langage (représentant un programme 
    ou une pile) en un texte équivalent. 
    Par exemple : [text (parse "1 2 +")) = "1 2 +"].
*)
let rec text (p:element list) : string =
  match p with
  | [] -> ""
  | [e] -> to_string e
  | hd::tl ->  (to_string hd)^(" ")^(text tl)

(* *********** Question 2 *********** *)

type prog = element list
type stack = element list

(* fonction auxiliaire : évaluation d'un opérateur binaire *)
(* Les operateurs qui peuvent etre evalues dans cette fonction sont ceux qui prennent systematiquement 2 et seulement 2 elements (Operateurs de Comparaison ou Arithmetiques) *)
let eval_binop op (e1:element) (e2:element) : element = match (e1 , e2) with
  (* Si les deux éléments sont des entiers... *)
  |(CST_INT a , CST_INT b) ->
      ( match op with
        | OP_ARITH ar ->
            (match ar with
             (* ... et l'opération est une multiplication, retourner a * b *)
             | TIMES  ->  CST_INT(a * b)
             (* ... et l'opération est une division, retourner a / b *)
             | DIV ->  CST_INT(a / b)
             (* ... et l'opération est une addition, retourner a + b *)
             | PLUS -> CST_INT(a + b)
             (* ... et l'opération est une soustraction, retourner a - b *)
             | MINUS -> CST_INT(a - b)
            )
        | OP_COMP comp ->
            (match comp with
             (* ... et l'opération est une égalité, retourner true si a = b, false sinon*)
             | EQUALS -> CST_BOOL (a = b)
             (* ... et l'opération est une différence, retourner true si a <> b, false sinon *)
             | DIFF -> CST_BOOL (a <> b)
             (* ... et l'opération est un inférieur, retourner true si a < b, false sinon *)
             | LT -> CST_BOOL (a < b)
             (* ... et l'opération est un supérieur, retourner true si a > b, false sinon *)
             | GT -> CST_BOOL (a > b)
            )
        |_ -> raise (Invalid_argument "Wrong OP in eval_binop (int)")
      )
  (* Si les deux éléments sont des booléens... *)
  |(CST_BOOL a , CST_BOOL b) ->
      (match op with
       | OP_COMP comp ->
           (match comp with
            (* ... et l'opération est une égalité, retourner true si a = b, false sinon *)
            | EQUALS -> CST_BOOL (a = b)
            (* ... et l'opération est une différence, retourner true si a <> b, false sinon *)
            | DIFF -> CST_BOOL (a <> b)
            (* ... on considere ici que les booleens ne sont pas comparables dans le sens ou l'un est superieur a l autre *)
            | _ -> raise (Invalid_argument "Wrong OP in eval_binop (bool)")
           )
       |_ -> raise (Invalid_argument "Wrong OP in eval_binop (bool)")
      )
  |_ -> raise (Invalid_argument "Wrong type of element in eval_binop")

(* Fonction auxiliaire pour évaluer une opération de pile (DUP DROP SWAP ou ROT), elle prend une pile et un opérateur *)
let eval_stackop (stk:stack) op : stack = match stk with
  (* Si la pile est vide, lever une exception *)
  |[] -> raise (Invalid_argument "Not enough args eval_stackop")
  |x::xs ->
      match op with
      | OP_PILE p ->
          (match p with
           (* Si l'opération est un DUP, retourner la pile avec l'élément en tête dupliqué *)
           | DUP -> x::stk
           (* Si l'opération est un DROP, retourner la pile sans l'élément en tête *)
           | DROP -> xs
           | SWAP -> (match xs with
               (* Si l'opération est un SWAP, inverser les deux premiers éléments de la pile *)
               | y::ys -> y::x::ys
               (* Si la pile a moins de deux éléments, lever une exception *)
               | _ -> raise (Invalid_argument "Not enough args eval_stackop")
             )
           | ROT -> (match xs with
               (* Si l'opération est un ROT, mettre le troisième élément en tête de la pile *)
               | y::z::zs -> y::z::x::zs
               (* Si la pile a moins de trois éléments, lever une exception *)
               | _ -> raise (Invalid_argument "Not enough args eval_stackop")
             )
          )
      | _ -> raise (Invalid_argument "Wrong OP in eval_stackop")

(* [step stk e] exécute l'élément [e] dans la pile [stk] 
   et retourne la pile résultante *) 
let step (stk:stack) (e:element) : stack =
  match e with
  (* Si l'élément est une constante, l'empiler sur la pile *)
  | CST_INT a -> e :: stk 
  | CST_BOOL b -> e :: stk
  | OP_ARITH op ->
      (match stk with
       (* Si la pile a moins de deux éléments, lever une exception *)
       | ([] |_::[]) -> raise (Invalid_argument "Not enough args in stk step")
       (* Sinon, évaluer l'opération aritmetique sur les deux elements du sommet de la pile *)
       | x1::x2::xs -> (eval_binop e x2 x1)::xs
      )
  | OP_COMP op ->
      (match stk with
       (* Si la pile a moins de deux éléments, lever une exception *)
       | ([] | _::[]) -> raise (Invalid_argument "Not enough args in stk step")
       (* Sinon, évaluer l'opération binaire sur les deux elements du sommet de la pile *)
       | x1::x2::xs-> (eval_binop e x2 x1)::xs
      )
  (* Si l'élément est une opération de pile, l'évaluer *)
  | OP_PILE op -> eval_stackop stk e
  (* les autres operateurs ne peuvent pas etre evalues a ce stade*)
  | _ -> raise (Invalid_argument "step")
           
(* *********** Question 3 *********** *)

(* La fonction 'calc' prend une pile (liste d'éléments sans définitions de noms ou operateurs conditionnels) 
et un programme et retourne la pile obtenue après l'exécution du programme, element par element *)
let rec calc (stk:stack) (p:prog) : stack = match p with 
  (* Si le programme est vide, on retourne la pile telle quelle *)
  |[] -> stk
  (* Si le programme contient encore des éléments, on prend le premier élément et on l'exécute
     avec la fonction 'step' sur la pile. On répète ensuite l'opération pour le reste du programme. *)
  |elem :: xs -> calc (step stk elem) xs
                   
(* *********** Question 4 *********** *)

type name = string 
(*Nous avons implémenté d'abord le dictionnaire comme étant une liste d'associations (nom, programme)
  puis nous avons transformé notre liste en ABR pour des raisons d'efficacité *)    
(*type dico = (name * prog) list*)
(*let empty : dico = []*)                               
type dico = Empty | Node of ((string * prog) * dico * dico)
let empty : dico = Empty
  
(* Versions initiales (non optimisées)
   
let add (x: name) (def: prog) (d: dico): dico =
  (* On commence par filtrer le dictionnaire en supprimant les définitions avec le même nom que celui passe en parametre *)
  let d = List.filter (fun (n, p) -> n <> x) d in
  (* Puis on ajoute la nouvelle définition en tête de la liste *)
  (x,def) :: d 

let rec lookup (n: name) (d: dico): prog =
 match d with
 (* si le dictionnaire est vide on lève l'exception 'Not_found' *)
 | [] -> raise Not_found 
 (* Si le dictionnaire contient au moins une définition, on vérifie si le nom de cette définition correspond à celui recherché.
    Si c'est le cas, on retourne le programme associé, sinon, on continue la recherche avec le reste du dictionnaire. *)
 | (name, prog) :: rest -> if name = n then prog else lookup n rest
   
*)


(* La fonction 'add' (Version optimisée) ajoute une nouvelle définition à un dictionnaire.Si une définition avec le même nom existe déjà, elle est remplacée par la nouvelle. *)
let rec add (x: name) (def: prog) (d: dico) =
  match d with
  | Empty -> Node ((x , def) , Empty , Empty)
  | Node ((n , p ), g , d) -> if n = x then Node ((x , def), g , d) else if x < n then Node ((n , p) , (add
                                                                                                          x def g) , d ) else Node ((n , p) , g , (add x def d))

(* La fonction 'lookup' (Version optimisée) recherche une définition par son nom dans un dictionnaire.
   Elle prend un nom et un dictionnaire, et retourne le programme associé à ce nom s'il existe, et lève l'exception 'Not_found' sinon. *)
let rec lookup (n: name) (d: dico) =
  match d with
  | Empty -> raise Not_found
  | Node ((k , v ), g , d) -> if k = n then v else if n < k then lookup n g else lookup n d
          
(* *********** Question 5 *********** *)

(* La fonction 'sep_n_prog' (separate name from program) evalue les definitions de nouveaux mots et les potentielles expressions imbriquees. *)
let rec sep_n_prog (p : prog) (dico : dico) : dico * prog =  
  match p with 
  |[] -> raise (Invalid_argument "wrong definition sep_n_prog")
  |x::xs -> let rec aux d acc xs = match xs with
      |[] -> raise (Invalid_argument "wrong definition sep_n_prog")
      |e :: xx -> (* si on rencontre ':' soit BEGIN_DEF on a une definition imbriquee (locale) alors on rappelle la fonction pour evaluer cette definition *)
          if e = BEGIN_DEF then let sd , sp = sep_n_prog xx d in aux sd [] sp 
          (* si on rencontre ';' soit END_DEF on a une fin de definition alors on ajoute la definition recuperee au dictionnaire et on renvoie la suite du programme (apres le ';') a evaluer *)    
          else if e = END_DEF then (add (to_string x) (List.rev acc) d , xx)
          (* sinon on continue a lire la definition *)                         
          else aux d (e :: acc) xx
      in aux dico [] xs 

(* La fonction 'eval_cond_op' évalue les opérations conditionnels et les potentielles expressions imbriquees. *)        
let rec eval_cond_op (stk : stack) (p:prog) : stack * prog = match stk with
  (* Si la pile est vide, on lève une exception. *)
  |[] -> raise (Invalid_argument ("eval_cond_op 1 " ^ text p))
  (* Si le sommet de la pile est 'true', on exécute la partie 'if' de l'opération conditionnelle.
   Si le sommet de la pile est 'false', on exécute la partie 'else' de l'opération conditionnelle.*)                                                            
  |CST_BOOL b::xs ->
      let rec aux buff acc b2 p = match p with
        |[] -> raise (Invalid_argument "eval_cond_op 2")
        |elem :: ys -> (* si on rencontre IF on a une operation conditionnelle imbriquee alors on rappelle la fonction pour evaluer cette operation *)
            if (elem = (OP_COND IF)) then let (xx , sp) = (eval_cond_op buff ys) in aux [] [] b sp
            (* si on rencontre THEN ou ENDIF on a une fin de condition alors on ajoute le bout de programme recupere a la suite du programme (apres le THEN/ENDIF) a evaluer *)
            else if (elem = (OP_COND THEN)) || (elem = (OP_COND ENDIF)) then (xs , (List.rev acc) @ ys)
            (* le booleen b determine si la partie du programme est conservee pour etre ensuite evaluee ou non.
             Dans le cas ou le sommet de la pile est true, le booleen b est a true avant le ELSE et passe a false apres ce qui permet d executer la partie IF de l operation
             Dans le cas ou le sommet de la pile est true, le booleen b est a false avant le ELSE et passe a true apres ce qui permet d executer la partie ELSE de l operation *)
            else if elem = (OP_COND ELSE) then aux buff acc (not b2) ys
            else if b2 then aux (elem::buff) (elem::acc) b2 ys else aux (elem::buff) acc b2 ys
      in aux [] [] b p 
  |_ -> raise (Invalid_argument "eval_cond_op 4") 


(* La fonction 'eval' évalue un programme avec un dictionnaire et une pile donnés. *)          
let rec eval (dico:dico) (stk:stack) (p:prog) : stack = match p with
  |[] -> stk
  |elem :: xs -> match elem with
    (* Pour une nouvelle définition, on l evalue avec la fonction sep_n_prog*)
    |BEGIN_DEF -> let d , reste = (sep_n_prog xs dico) in eval d stk reste 
    (* Pour une opération conditionnelle, on l'évalue avec la fonction 'eval_cond_op' *)
    |OP_COND IF -> let s , pg = eval_cond_op stk xs in eval dico s pg
    (* Pour les operateurs binaires, les operateurs de pile ou les constantes on les évalue avec la fonction 'step'. *)
    |OP_COMP o -> eval dico (step stk elem) xs
    |OP_ARITH o -> eval dico (step stk elem) xs
    |OP_PILE o -> eval dico (step stk elem) xs
    |CST_INT o -> eval dico (step stk elem) xs
    |CST_BOOL o -> eval dico (step stk elem) xs
    (* Pour une constante string (un mot), on cherche sa définition dans le dictionnaire puis on évalue le programme correspondant récupéré par la fonction 'lookup'. *)
    |CST_STRING mot -> eval dico stk ((lookup mot dico) @ xs)
    (* Si l'élément ne correspond à aucun des cas précédents, on lève une exception. *)
    |_ -> raise (Invalid_argument "eval") 
 
let jeux_de_test = [ (": fact dup 1 > if dup 1 - fact * then ; 6 fact", "720") ]


(* *********** Question 6 *********** *)

let carre n = 
  Printf.sprintf ": carre dup * ; %d carre" n 

let fib n = 
  Printf.sprintf ": fib DUP 1 > IF 1 - DUP 1 - fib SWAP fib + THEN ; %d fib" n
    
let fib2 n = (* autre version *)
  ": fib DUP 1 > IF 1 - DUP 1 - fib SWAP fib + THEN ; " ^ string_of_int(n) ^ " fib"
    
(* *********** Question 7 *********** *)

let jeux_de_test = [
  ("1 2 +", "3");                              (* Addition simple *)
  ("3 4 *", "12");                             (* Multiplication simple *)
  ("6 DUP *", "36");                           (* Carré d'un nombre *)
  ("5 1 2 ROT + *", "12");                     (* Utilisation de ROT *)
  ("2 3 DUP * SWAP 1 + /", "3");               (* Manipulation complexe de la pile avec SWAP *)
  (": fact DUP 1 > IF DUP 1 - fact * THEN ; 6 fact", "720"); (* Calcul récursif : factoriel *)
  (": fib DUP 1 > IF 1 - DUP 1 - fib SWAP fib + THEN ; 5 fib", "5") (* Calcul récursif : Fibonacci *)
] ;;
