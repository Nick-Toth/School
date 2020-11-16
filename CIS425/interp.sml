(* **********************************************************
\\ File: interp.sml
// Name: Nick G. Toth
\\ Date: Nov 15, 2020
// Email: ngt@uoregon.edu
\\
\\ Overview: This file contains an intpreter (for which
// some code, including essentially all of the parser, was
\\ provided by my CIS 425 Instructor at UO, Zena Ariola).
// 
\\ While studying for the second midterm, I extended the
// interpreter's functionality to include rational numbers.
\\ Rational numbers are represented here as (int, int)
// where the second int is non-negative. It's not very
\\ fancy. I haven't written code to reduce the rationals,
// so they're not super nice to work with. Future extension?!
\\ 
// I also implemented division, multiplication, and less-than
\\ comparison, and I extended addition to work on rationals.
//
\\ Finally, I extended all of the aforementioned binary
// operators so that each of them work on
\\    (int*int) --original functionality,
//    (int*rat),
\\    (rat*int),
//    and (rat*rat).
\\
// I also extended ISZERO for the rational numbers.
\\
// If you're in 425, note that this interp file will not work
\\ with the old parser from homework 4. I've had to make a
// few small changes which should all be commented in the
\\ parser.sml file on my github page in /School/CIS425/.
//
\\ Also note that these features have all been implemented
// for both the dynamic and static interpreters, interp and
\\ interp_static, respectively. 
//
\\ Also BEWARE ACCIDENTALLY OPENING COMMENTS WITH MULT!!!
// I was stupid; I forgot that ( * opens a comment in sml,
\\ and I implemented multiplication with the * operator,
// so to compute something like 3*5, you need to separate
\\ the space from the paren. i.e. ( * 3 5) = 15 or
// ( * 3 5 ) = 15.
\\
// And sorry for adding so many things.
\\
* **********************************************************)



(* FOR ME: export PATH="$PATH:/usr/local/smlnj/bin/" *)

(* Aha! You can't break the interpreter up into static,
  dynamic, and common parts because the leaves are not common.
  i.e. it's not clear which interpreter to call from the common interpreter. *)


(*  Concrete syntax
e :: x | n | true | false | succ | pred | iszero | if e then e else e
       | fn x => e | e e | (e) | let x = e in e
*)

(* The following definition in in the parser module 
datatype term = AST_ID of string | AST_NUM of int  | AST_BOOL of bool| 
                AST_SUCC | AST_PRED | AST_ISZERO | AST_ADD |
                AST_IF of term * term * term | 
                AST_FUN of string * term | AST_APP of term * term | 
                AST_LET of (string * term * term) | AST_ERROR of string |
                AST_REC of (string * term)
*)



use "parser.sml";

datatype env = Env of (string -> result)
and result = RES_ERROR of string
           | RES_NUM of int
           | RES_RAT of int*int (* added by ngt *)
           | RES_BOOL of bool
           | RES_SUCC
           | RES_PRED
           | RES_ISZERO
           | RES_FUN of (string * term)
           | RES_CLOSURE of (string*term*env);


exception ZeroDivError of string (* added by ngt *)
exception Error of string
exception UnboundID of string ;
exception Not_implemented


fun emptyenvFun  (x : string) : result = raise UnboundID x;

val emptyenv = Env emptyenvFun


(* unwrap env datatype to apply env to x. *)
fun lookup (Env e) x = e x


(* e [x=v]  update e x v *)

(*  update : env -> string -> result -> string -> result  *)
fun update env (x : string) (value : result) =
                  fn y => if x = y then value else lookup env y


(* Rules 1-3 *)
fun interp  (env, AST_NUM n)  = RES_NUM n
|   interp  (env, AST_ID x)   = lookup env x
|   interp  (env, AST_BOOL b) = RES_BOOL b
|   interp  (env, AST_SUCC)   = RES_SUCC
|   interp  (env, AST_PRED)   = RES_PRED
    (* Note that RES_RAT is used only where / is used. *)
|   interp  (env, AST_ISZERO) = RES_ISZERO

|   interp (env, AST_IF (e1, e2, e3)) = (
      case interp (env, e1) of
          RES_BOOL false => interp  (env, e3)
        | _              => raise  (Error "if condition non-bool!") )

|   interp (env, AST_FUN(x, body)) = RES_FUN (x, body)

|   interp (env, AST_LET (x, e1, e2)) = let val v1 = interp (env, e1)
                                        in interp (Env (update env x v1), e2)
                                        end
(***********************************************************************************************)
(* added by ngt *)
|   interp (env, AST_APP(AST_APP(AST_ADD, e1), e2)) =  (* + e1 e2 *)
      let val v1 = interp (env, e1) 
          val v2 = interp (env, e2)
      in
        case (v1, v2) of
          (* The good stuff. *)
            (RES_NUM a,      RES_NUM b)      => RES_NUM (a+b)          (* int+int -> int *)
          | (RES_RAT (a, b), RES_NUM c)      => RES_RAT (b*c+a, b)     (* rat+int -> rat *)
          | (RES_NUM a,      RES_RAT (b, c)) => RES_RAT (a*c+b, c)     (* int+rat -> rat *)
          | (RES_RAT (a, b), RES_RAT (c, d)) => RES_RAT (a*d+b*c, b*d) (* rat+rat -> rat *)

          (* Handle first-argument type errors. *)
          | (_, RES_NUM b)      => raise Error "Invalid type supplied to the first argument of +"
          | (_, RES_RAT (b, c)) => raise Error "Invalid type supplied to the first argument of +"

          (* Handle second-argument type errors. *)
          | (RES_NUM a,      _) => raise Error "Invalid type supplied to the second argument of +"
          | (RES_RAT (a, b), _) => raise Error "Invalid type supplied to the second argument of +"

          (* Handle first-and-second-argument type errors. *)
          | _ => raise Error "Invalid type supplied to both arguments of +"
      end

(* Handle argument count errors. *)
|   interp (env, AST_APP(AST_ADD, e1)) = raise (Error "not enough arguments for +") (* + e1 *)

(***********************************************************************************************)
(* added by ngt *)
|   interp (env, AST_APP(AST_APP(AST_MUL, e1), e2)) =  (* * e1 e2 *)
      let val v1 = interp (env, e1) 
          val v2 = interp (env, e2)
      in
        case (v1, v2) of
          (* The good stuff. *)
            (RES_NUM a,      RES_NUM b)      => RES_NUM (a*b)      (* int+int -> int *)
          | (RES_RAT (a, b), RES_NUM c)      => RES_RAT (a*c, b)   (* rat+int -> rat *)
          | (RES_NUM a,      RES_RAT (b, c)) => RES_RAT (a*b, c)   (* int+rat -> rat *)
          | (RES_RAT (a, b), RES_RAT (c, d)) => RES_RAT (a*c, b*d) (* rat+rat -> rat *)

          (* Handle first-argument type errors. *)
          | (_, RES_NUM b)      => raise Error "Invalid type supplied to the first argument of *"
          | (_, RES_RAT (b, c)) => raise Error "Invalid type supplied to the first argument of *"

          (* Handle second-argument type errors. *)
          | (RES_NUM a,      _) => raise Error "Invalid type supplied to the second argument of *"
          | (RES_RAT (a, b), _) => raise Error "Invalid type supplied to the second argument of *"

          (* Handle first-and-second-argument type errors. *)
          | _ => raise Error "Invalid type supplied to both arguments of *"
      end
|   interp (env, AST_APP(AST_MUL, e1)) = raise (Error "not enough arguments for *") (* * e1 *)
(***********************************************************************************************)
(* added by ngt *)
|   interp (env, AST_APP(AST_APP(AST_DIV, e1), e2)) =  (*  / e1 e2   *)
      let val v1 = interp (env, e1) 
          val v2 = interp (env, e2)
      in
        case (v1, v2) of
          (* Handle division by zero errors *)
            (RES_NUM _,      RES_NUM 0)      => raise ZeroDivError "a/0"
          | (RES_RAT (_, 0), RES_NUM _)      => raise ZeroDivError "(a/0)/b     = a/0b"
          | (RES_RAT (_, _), RES_NUM 0)      => raise ZeroDivError "(a/b)/0     = a/b0"
          | (RES_NUM _,      RES_RAT (0, _)) => raise ZeroDivError "a/(0/c)     = ca/0"
          | (RES_RAT (_, 0), RES_RAT (_, _)) => raise ZeroDivError "(a/0)/(b/c) = ac/0b"
          | (RES_RAT (_, _), RES_RAT (0, _)) => raise ZeroDivError "(a/b)/(0/c) = ca/b0"

          (* The good stuff. *)
          | (RES_NUM a,      RES_NUM b)      => RES_RAT (a, b)     (* int/int -> rat *)
          | (RES_RAT (a, b), RES_NUM c)      => RES_RAT (a, b*c)   (* rat/int -> rat *)
          | (RES_NUM a,      RES_RAT (b, c)) => RES_RAT (a*c, b)   (* int/rat -> rat *)
          | (RES_RAT (a, b), RES_RAT (c, d)) => RES_RAT (a*d, b*c) (* rat/rat -> rat *)


          (* Handle first-argument type errors. *)
          | (_, RES_NUM b)      => raise Error "Invalid type supplied to the first argument of /"
          | (_, RES_RAT (b, c)) => raise Error "Invalid type supplied to the first argument of /"

          (* Handle second-argument type errors. *)
          | (RES_NUM a,      _) => raise Error "Invalid type supplied to the second argument of /"
          | (RES_RAT (a, b), _) => raise Error "Invalid type supplied to the second argument of /"

          (* Handle first-and-second-argument type errors. *)
          | _ => raise Error "Invalid type supplied to both arguments of /"
      end
|   interp (env, AST_APP(AST_DIV, e1)) = raise (Error "not enough arguments for /") (*   / e1   *)
(***********************************************************************************************)
(* added by ngt *)
|   interp (env, AST_APP(AST_APP(AST_LESS, e1), e2)) =  (*   < e1 e2   *)
      let val v1 = interp (env, e1) 
          val v2 = interp (env, e2)
      in
        case (v1, v2) of
          (* The good stuff. *)
            (RES_NUM a,      RES_NUM b)      => RES_BOOL (a < b)     (* int<int -> bool *)
          | (RES_RAT (a, b), RES_NUM c)      => RES_BOOL (a < b*c)   (* rat<int -> bool *)
          | (RES_NUM a,      RES_RAT (b, c)) => RES_BOOL (a*c < b)   (* int<rat -> bool *)
          | (RES_RAT (a, b), RES_RAT (c, d)) => RES_BOOL (a*d < b*c) (* rat<rat -> bool *)

          (* Handle first-argument type errors. *)
          | (_, RES_NUM b)      => raise Error "Invalid type supplied to the first argument of <"
          | (_, RES_RAT (b, c)) => raise Error "Invalid type supplied to the first argument of <"

          (* Handle second-argument type errors. *)
          | (RES_NUM a,      _) => raise Error "Invalid type supplied to the second argument of <"
          | (RES_RAT (a, b), _) => raise Error "Invalid type supplied to the second argument of <"

          (* Handle first-and-second-argument type errors. *)
          | _ => raise Error "Invalid type supplied to both arguments of +"
      end
|   interp (env, AST_APP(AST_LESS, e1)) = raise (Error "not enough arguments for <") (*   < e1   *)

(***********************************************************************************************)

(* Function Application iii *)
|   interp (env, AST_APP (e1, e2)) = 
    ( case interp (env, e1) of (* in other words, find e1 to be applied to e2. *)

        RES_FUN (formal, body) => let val actual  = interp (env, e2) 
                                      val new_env1 = Env (update env formal actual)
                                  in interp (new_env1, body)
                                  end

      | RES_SUCC => ( case interp (env, e2) of
                          RES_NUM n => RES_NUM (n+1)
                        | _         => raise (Error "succ non-number"))

      | RES_PRED => ( case interp (env,e2) of
                          RES_NUM n => RES_NUM (n+1)
                        | _         => raise (Error "pred non-number"))

      | RES_ISZERO => ( case interp (env,e2) of
                            RES_NUM n      => RES_BOOL (n = 0)
                          | RES_RAT (a, b) => RES_BOOL (a = 0) (* added by ngt *)
                          | _ => raise (Error "iszero non-number"))

      | _ => raise (Error "apply non-function")
    )

|   interp (env, AST_ERROR s)       = raise (Error s)
|   interp (env, _)                 = raise Not_implemented;




val test_string = "let x = 2 in let f = fn z => x  in let  x = 100 in (f x) end end end";

val test = AST_LET ("x",  AST_NUM 2,
            AST_LET ("f", AST_FUN ("z", AST_ID "x"),
             AST_LET("x", AST_NUM 100, 
              AST_APP (AST_ID "f", AST_ID "x"))));

val resul_test = interp (emptyenv, test);

val x = parsestr test_string;



(* 
interp: env*term -> result
interp_1: string -> result
*)
fun interp_1 s = interp (emptyenv, parsestr s)
                  handle (Error z) =>  RES_ERROR z
                 |       (UnboundID x) => RES_ERROR("Variable Unbound " ^ x );




interp_1 test_string;


(*

val test1 = "let x = 1 in \
              \let f = fn y => let  w = + y 1 \
                \                in fn z => + (+ (+ x y) z) w \
                  \             end \
                  \in let x = 8 \
                    \ in let g = f 6 \
                      \   in let y = 5 \
                        \    in g 3 \
                        \ end end end end end";
val t1 = interp_1 test1;
(* Error - Unbound variable w -- note: very similar MT Q8,9!!!!! *)



val test2 = "let f  = fn  g => let x = 3 in g 2 end \
               \in  let x = 4  \
                \    in let h = fn y => + x y   \
                 \      in f h \
                   \    end end end";

val t2 = interp_1 test2;
*)


(* My tests *)

(* 5 + 3 = 8 *)
val test3 = "let n = 3 \
             \ in \
               \let f = fn x => + x n \
               \ in \
                 \ f 5 \
               \ end end"

val t3 = interp_1 test3;



(* 5 * 3 = 15 *)
val mul_test_1 = "let n = 3 \
                \ in \
                  \ let f = fn x => * x n \
                  \ in \
                    \ f 5 \
                \ end end"

val t4 = interp_1 mul_test_1;


(* 5 < 3 = false *)
val less_test_1 = "let n = 3 \
                 \ in \
                   \ let f = fn x => < x n \
                   \ in \
                     \ f 5 \
                 \ end end"

val t5 = interp_1 less_test_1;


(* 3 < 5 = true *)
val less_test_2 = "let n = 5 \
                 \ in \
                   \ let f = fn x => < x n \
                   \ in \
                     \ f 3 \
                 \ end end"

val t6 = interp_1 less_test_2;








(***********************************************************************)
(* STATIC INTERPRETER *)

fun
    interp_static (env, AST_SUCC)          = RES_SUCC
|   interp_static (env, AST_PRED)          = RES_PRED
|   interp_static (env, AST_ISZERO)        = RES_ISZERO
|   interp_static (env, AST_NUM n)         = RES_NUM n
|   interp_static (env, AST_BOOL b)        = RES_BOOL b
|   interp_static (env, AST_ID x )         = lookup env x 

(* Rules 4,5 *)
|   interp_static (env, AST_IF (e1, e2, e3)) = (
      case interp_static (env, e1) of
          RES_BOOL true  => interp_static  (env,e2)
        | RES_BOOL false => interp_static  (env,e3)
        | _              => raise  (Error "if condition non-bool!") )

(* Function Definition (lambda) *)
|   interp_static (env, AST_FUN(formal, body)) = (* RES_FUN (x,body) *)
      (*  $$ CHANGE #1 $$  *)
      (* Rule #10 a:    env |- (fn x => e) --> ((fn x => e), env) *) 
      RES_CLOSURE(formal, body, env)
      (*  $$ END CHANGE #1 $$  *)

(* Variable Declaration  *)
|   interp_static  (env, AST_LET (x, e1, e2)) = let val v1 = interp_static (env, e1)
                                                in interp_static (Env (update env x v1), e2)
                                                end

(***********************************************************************************************)
(* added by ngt *)
|   interp_static (env, AST_APP(AST_APP(AST_ADD, e1), e2)) =  (* + e1 e2 *)
      let val v1 = interp_static (env, e1) 
          val v2 = interp_static (env, e2)
      in
        case (v1, v2) of
          (* The good stuff. *)
            (RES_NUM a,      RES_NUM b)      => RES_NUM (a+b)          (* int+int -> int *)
          | (RES_RAT (a, b), RES_NUM c)      => RES_RAT (b*c+a, b)     (* rat+int -> rat *)
          | (RES_NUM a,      RES_RAT (b, c)) => RES_RAT (a*c+b, c)     (* int+rat -> rat *)
          | (RES_RAT (a, b), RES_RAT (c, d)) => RES_RAT (a*d+b*c, b*d) (* rat+rat -> rat *)

          (* Handle first-argument type errors. *)
          | (_, RES_NUM b)      => raise Error "Invalid type supplied to the first argument of +"
          | (_, RES_RAT (b, c)) => raise Error "Invalid type supplied to the first argument of +"

          (* Handle second-argument type errors. *)
          | (RES_NUM a,      _) => raise Error "Invalid type supplied to the second argument of +"
          | (RES_RAT (a, b), _) => raise Error "Invalid type supplied to the second argument of +"

          (* Handle first-and-second-argument type errors. *)
          | _ => raise Error "Invalid type supplied to both arguments of +"
      end

(* Handle argument count errors. *)
|   interp_static (env, AST_APP(AST_ADD, e1)) = raise (Error "not enough arguments for +") (* + e1 *)

(***********************************************************************************************)
(* added by ngt *)
(* ( * operator ) added by ngt - see parser.sml for other changes to implement mul. *)
|   interp_static (env, AST_APP(AST_APP(AST_MUL, e1), e2)) =  (* * e1 e2 *)
      let val v1 = interp_static (env, e1) 
          val v2 = interp_static (env, e2)
      in
        case (v1, v2) of
          (* The good stuff. *)
            (RES_NUM a,      RES_NUM b)      => RES_NUM (a*b)      (* int+int -> int *)
          | (RES_RAT (a, b), RES_NUM c)      => RES_RAT (a*c, b)   (* rat+int -> rat *)
          | (RES_NUM a,      RES_RAT (b, c)) => RES_RAT (a*b, c)   (* int+rat -> rat *)
          | (RES_RAT (a, b), RES_RAT (c, d)) => RES_RAT (a*c, b*d) (* rat+rat -> rat *)

          (* Handle first-argument type errors. *)
          | (_, RES_NUM b)      => raise Error "Invalid type supplied to the first argument of *"
          | (_, RES_RAT (b, c)) => raise Error "Invalid type supplied to the first argument of *"

          (* Handle second-argument type errors. *)
          | (RES_NUM a,      _) => raise Error "Invalid type supplied to the second argument of *"
          | (RES_RAT (a, b), _) => raise Error "Invalid type supplied to the second argument of *"

          (* Handle first-and-second-argument type errors. *)
          | _ => raise Error "Invalid type supplied to both arguments of *"
      end

(* Handle argument count errors. *)
|   interp_static (env, AST_APP(AST_MUL, e1)) = raise (Error "not enough arguments for *") (* * e1 *)
(***********************************************************************************************)
(* added by ngt *)
|   interp_static (env, AST_APP(AST_APP(AST_DIV, e1), e2)) =  (*  / e1 e2   *)
      let val v1 = interp_static (env, e1) 
          val v2 = interp_static (env, e2)
      in
        case (v1, v2) of
          (* Handle division by zero errors *)
            (RES_NUM _,      RES_NUM 0)      => raise ZeroDivError "a/0"
          | (RES_RAT (_, 0), RES_NUM _)      => raise ZeroDivError "(a/0)/b     = a/0b"
          | (RES_RAT (_, _), RES_NUM 0)      => raise ZeroDivError "(a/b)/0     = a/b0"
          | (RES_NUM _,      RES_RAT (0, _)) => raise ZeroDivError "a/(0/c)     = ca/0"
          | (RES_RAT (_, 0), RES_RAT (_, _)) => raise ZeroDivError "(a/0)/(b/c) = ac/0b"
          | (RES_RAT (_, _), RES_RAT (0, _)) => raise ZeroDivError "(a/b)/(0/c) = ca/b0"

          (* The good stuff. *)
          | (RES_NUM a,      RES_NUM b)      => RES_RAT (a, b)     (* int/int -> rat *)
          | (RES_RAT (a, b), RES_NUM c)      => RES_RAT (a, b*c)   (* rat/int -> rat *)
          | (RES_NUM a,      RES_RAT (b, c)) => RES_RAT (a*c, b)   (* int/rat -> rat *)
          | (RES_RAT (a, b), RES_RAT (c, d)) => RES_RAT (a*d, b*c) (* rat/rat -> rat *)


          (* Handle first-argument type errors. *)
          | (_, RES_NUM b)      => raise Error "Invalid type supplied to the first argument of /"
          | (_, RES_RAT (b, c)) => raise Error "Invalid type supplied to the first argument of /"

          (* Handle second-argument type errors. *)
          | (RES_NUM a,      _) => raise Error "Invalid type supplied to the second argument of /"
          | (RES_RAT (a, b), _) => raise Error "Invalid type supplied to the second argument of /"

          (* Handle first-and-second-argument type errors. *)
          | _ => raise Error "Invalid type supplied to both arguments of /"
      end

|   interp_static (env, AST_APP(AST_DIV, e1)) = raise (Error "not enough arguments for /") (*   / e1   *)
(***********************************************************************************************)
(* added by ngt *)
(* ( < operator ) added by ngt - see parser.sml for other changes to implement less. *)
|   interp_static (env, AST_APP(AST_APP(AST_LESS, e1), e2)) =  (*   < e1 e2   *)
      let val v1 = interp_static (env, e1) 
          val v2 = interp_static (env, e2)
      in
        case (v1, v2) of
          (* The good stuff. *)
            (RES_NUM a,      RES_NUM b)      => RES_BOOL (a < b)     (* int<int -> bool *)
          | (RES_RAT (a, b), RES_NUM c)      => RES_BOOL (a < b*c)   (* rat<int -> bool *)
          | (RES_NUM a,      RES_RAT (b, c)) => RES_BOOL (a*c < b)   (* int<rat -> bool *)
          | (RES_RAT (a, b), RES_RAT (c, d)) => RES_BOOL (a*d < b*c) (* rat<rat -> bool *)

          (* Handle first-argument type errors. *)
          | (_, RES_NUM b)      => raise Error "Invalid type supplied to the first argument of <"
          | (_, RES_RAT (b, c)) => raise Error "Invalid type supplied to the first argument of <"

          (* Handle second-argument type errors. *)
          | (RES_NUM a,      _) => raise Error "Invalid type supplied to the second argument of <"
          | (RES_RAT (a, b), _) => raise Error "Invalid type supplied to the second argument of <"

          (* Handle first-and-second-argument type errors. *)
          | _ => raise Error "Invalid type supplied to both arguments of +"
      end

|   interp_static (env, AST_APP(AST_LESS, e1)) = raise (Error "not enough arguments for <") (*   < e1   *)
(***********************************************************************************************)

(* Function Application iii *)
| interp_static (env, AST_APP (e1, e2)) = 
    ( case interp_static (env, e1) of

        RES_CLOSURE(x, b, env') => interp_static(Env (update env' x (interp_static (env, e2))), b)

      | RES_SUCC => ( case interp_static (env, e2) of
                          RES_NUM n => RES_NUM (n+1)
                        | _         => raise (Error "succ non-number")
                    )

      | RES_PRED => ( case interp_static (env, e2) of
                          RES_NUM n => RES_NUM (n+1)
                        | _         => raise (Error "pred non-number")
                    )

      | RES_ISZERO => ( case interp_static (env,e2) of
                            RES_NUM n      => RES_BOOL (n = 0)
                          | RES_RAT (a, b) => RES_BOOL (a = 0) (* added by ngt *)
                          | _ => raise (Error "iszero non-number"))

      | _ => raise (Error "apply non-function")
    )

 |  interp_static (env, AST_ERROR s)       = raise (Error s)

 |  interp_static (env, _)                = raise Not_implemented;




(* (* Closure rules for static interpreter (hw 4, exercise 2) *)

(* Rule #10 a *)
    env |- (fn x => e) --> ((fn x => e), env)

(* Rule #11 *)  
    env |- e1-->((fn x => e), env1)         env |- e2-->v1 env1[x=v1]|- e-->v
    -------------------------------------------------------------------------
                            env |- e1 e2 --> v
*)

(*
fun interp_2 s = interp_static (emptyenv, parsestr s)
                   handle (Error z) =>  RES_ERROR z
                 | (UnboundID x)    => RES_ERROR("Variable Unbound " ^ x );


val test2_1 = "let x = 1 in \
              \let f = fn y => let  w = + y 1 \
                \                in fn z => + (+ (+ x y) z) w \
                  \             end \
                  \in let x = 8 \
                    \ in let g = f 6 \
                      \   in let y = 5 \
                        \    in g 3 \
                        \ end end end end end";
val t2_1 = interp_2 test2_1;
(* Error - Unbound variable w -- note: very similar MT Q8,9!!!!! *)



val test2_2 = "let f  = fn  g => let x = 3 in g 2 end \
               \in  let x = 4  \
                \    in let h = fn y => + x y   \
                 \      in f h \
                   \    end end end";

val t2_2 = interp_2 test2_2;



val test2_3 = "let n = 3 \
             \ in \
               \let f = fn x => + x n \
               \ in \
                 \ f 5 \
               \ end end"

val t2_3 = interp_2 test2_3;




(* Test static interp multiply than operator *)
(* 5*3 = 15 *)
val test2_4 = "let n = 5 \
             \ in \
               \let f = fn x => * x n \
               \ in \
                 \ f 3 \
               \ end end"

val t2_4 = interp_2 test2_4;


(* Test static interp less than operator *)
(* 5 < 3 = false *)
val test2_4 = "let n = 3 \
             \ in \
               \let f = fn x => < x n \
               \ in \
                 \ f 5 \
               \ end end"

val t2_4 = interp_2 test2_4;

(* 3 < 5 = true *)
val test2_4 = "let n = 5 \
             \ in \
               \let f = fn x => < x n \
               \ in \
                 \ f 3 \
               \ end end"

val t2_4 = interp_2 test2_4;
*)


(*
(* DYNAMIC *) (* (10)    env |- (fn x => e) --> (fn x => e)     *)
|   interp (env, AST_FUN(x, body)) = RES_FUN (x, body)


(* Static *) (* (10a):    env |- (fn x => e) --> ((fn x => e), env) *) 
|   interp_static (env, AST_FUN(x,body)) = (* RES_FUN (x,body) *)
      (* Rule #10 a:    env |- (fn x => e) --> ((fn x => e), env) *) 
      RES_CLOSURE(x, body, env)
*)



(* Tests from Nov 2 Lecture. *)
(*
val res9 = interp_1 "succ true" (* error - false? just kidding. This is also stupid. Also you did these backwards, as you will see very soon. *)
val res10 = interp_1 "1 2" (* error 1 applied to 2 is stupid. *)
val res11 = interp_1 "if 1 then 2 else false" (* error - 1 is not a bool, and returns int and bool. *)
val res12 = interp_1 "if true then 2 else false" (* Compiles, but error - returns int and bool. *)
val res13 = interp_1 "fn x => if x then (+ 2 x) else 4" (* Compiles, but error - x is a bool and x is an int. *)
val res14 = interp_1 "(fn x => if x then (+ 2 x) else 4) true" (* error - (+ 2 true) *)
*)
(* OUTPUT 
val res9 = RES_ERROR "succ non-number" : result
val res10 = RES_ERROR "apply non-function" : result
val res11 = RES_ERROR "if condition non-bool!" : result
val res12 = RES_NUM 2 : result
val res13 =
  RES_FUN
    ("x",
     AST_IF
       (AST_ID "x",AST_APP (AST_APP (AST_ADD,AST_NUM 2),AST_ID "x"),AST_NUM 4))
  : result
val res14 = RES_ERROR "not a number" : result
*)

(*
(* My test *)
val my_res1 = interp_1 "(fn x => if (x=5) then (+ 2 x) else 4) 5" (* Error (=) *)
val my_res2 = interp_1 "(fn x => if (iszero x) then 4 else (+ 2 x)) 5" (* 2+5=7 *)
val my_res3 = interp_1 "(fn x => if (iszero x) then 4 else (+ 2 x)) 0" (* 4 *)

(* interp 2 (static) *)
val my_res4 = interp_2 "(fn x => if (< x 5) then 4 else (+ 2 x)) 3" (* 4 *)
val my_res5 = interp_2 "(fn x => if (< x 5) then 4 else (+ 3 x)) 7" (* 10 *)
*)








(***************************************************************************)


(* Nick's Selected Rational Number Interpreter Tests. *)



(* BEWARE ACCIDENTALLY OPENING COMMENTS WITH MULT!!! HAHAHA OOPS! *)
(* interp 1 again (dynamic) *)
val my_res6 = interp_1 "(/ 8 2) " (* (8, 2) *)
val my_res7 = interp_1 "(+ (/ 8 2) 5)" (* (8, 2) + 5 = (18,2) *)


(* Correct *)
val my_res8 = interp_1 "( * (+ (/ 8 ( * 2 (/ 5 13))) 1) (/ 5 3))" (* (570,30) (* note 570/30 = 19 totally by accident. *) (* equivalent computation: (5/3)* ((8/(2*(5/13))) + 1)*)*)
val my_res9 = interp_1 "(+ (/ 8 (/ 3 7)) 5)" (* (71/3) = 23.666... (* equivalent computation: (8/(3/7))+5*)*)
(* val my_res10 = interp_1 "( * (+ (/ 8 ( * 2 (/ 5 0))) 1) (/ 5 3))" (* Error - div by 0. *) *)


(* val my_res11 = interp_1 "( * (+ (/ 8 ( * 0 (/ 5 13))) 1) (/ 5 3))" (* error - (520, 0) *) *)

(* val my_res13 = interp_1 "(/ 8 ( * 0 (/ 5 13)))" (*  Error - div by 0. *)*)
(* val my_res12 = interp_1 "( / 8 ( * 0 1) )" (*  Error - div by 0. *) *)


fun interp_2 s = interp_static (emptyenv, parsestr s)
                   handle (Error z) =>  RES_ERROR z
                 | (UnboundID x)    => RES_ERROR("Variable Unbound " ^ x );


val my_res14 = interp_2 "(/ 8 2) " (* (8, 2) *)
val my_res15 = interp_2 "(+ (/ 8 2) 5)" (* (8, 2) + 5 = (18,2) *)

val my_res16 = interp_2 "( * (+ (/ 8 ( * 2 (/ 5 13))) 1) (/ 5 3))" (* (570,30) (* note 570/30 = 19 totally by accident. *) (* equivalent computation: (5/3)* ((8/(2*(5/13))) + 1)*)*)
val my_res17 = interp_2 "(+ (/ 8 (/ 3 7)) 5)" (* (71/3) = 23.666... (* equivalent computation: (8/(3/7))+5*)*)
(* val my_res12 = interp_1 "( / 8 ( * 0 1) )" (*  Error - div by 0. *) *)
