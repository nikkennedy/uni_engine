(UMLStudio "6.2" project)
(repository "" 0)
(notation "UML.not")
(genProfile 50 1 ("" "" "" "" "") "" 0 ("" "" "" "" "") ("" "" "" "" "") 700 
            360 207 441 70 50 80 50 80 60 0 0)
(codeFiles)
(docFiles)
(otherFiles)
(revFiles 1 "E:\Stuff\working\horizon\global\include\object.h")
(masters (master "O-!nUJ;.!}KG5!!" 1 3 "CBullet" "" "" "" "" "" 1 nil nil nil 
                 nil 10 "") 
         (master "J-!(VJ;.!}KG5!!" 1 3 "CCollectable" "" "" "" "" "" 1 nil nil 
                 nil nil 10 "") 
         (master "E-!-VJ;.!}KG5!!" 1 3 "CBot" "" "" "" "" "" 1 nil nil nil nil 
                 10 "") 
         (master "@-!3VJ;.!}KG5!!" 1 3 "CPlayer" "" "" "" "" "" 1 nil nil nil 
                 nil 10 "") 
         (master "v,!oVJ;.!}KG5!!" 1 4 "CObject" "" "" "" "" "" 1 
                 (("shape" "CHrzParser*" "" "" "" 
                           "objects can have a physical representation (i.e. a bullet)" 
                           3 16 "x,!oVJ;.!}KG5!!") 
                  ("md2_model" "CMD2*" "" "" "" 
                               "objects can have a MD2 as their representation" 
                               3 16 "y,!oVJ;.!}KG5!!") 
                  ("height" "float" "" "" "" "300 for player" 3 16 
                            "z,!oVJ;.!}KG5!!") 
                  ("speed" "float" "" "" "" "30 for player" 3 16 
                           "{,!oVJ;.!}KG5!!") 
                  ("step_amount" "float" "" "" "" "100 for player" 3 16 
                                 "|,!oVJ;.!}KG5!!") 
                  ("step_unit" "float" "" "" "" "5 for player" 3 16 
                               "},!oVJ;.!}KG5!!") 
                  ("width" "float" "" "" "" "30 for player" 3 16 
                           "~,!oVJ;.!}KG5!!") 
                  ("falltime" "int" "" "" "" 
                              "amount to frames so far in which object fallen" 
                              3 16 "!-!oVJ;.!}KG5!!") 
                  ("mass" "float" "" "" "" "mass of object // 60 for player" 3 
                          16 "^"-!oVJ;.!}KG5!!") 
                  ("gravity" "float" "" "" "" 
                             "value of accelearation due to gravity for object" 
                             3 16 "#-!oVJ;.!}KG5!!") 
                  ("active" "int" "" "" "" 
                            "is this object active? (used for bullets etc.)" 3 
                            16 "$-!oVJ;.!}KG5!!") 
                  ("hitable" "int" "" "" "" "does this object hitable?" 3 16 
                             "%-!oVJ;.!}KG5!!") 
                  ("location" "CVert3D" "" "" "" "" 3 16 "&-!oVJ;.!}KG5!!") 
                  ("orientation" "CVert3D" "" "" "" "" 3 16 "'-!oVJ;.!}KG5!!") 
                  ("start_position" "CVert3D" "" "" "" "" 3 16 
                                    "(-!oVJ;.!}KG5!!") 
                  ("end_position" "CVert3D" "" "" "" "" 3 16 ")-!oVJ;.!}KG5!!") 
                  ("dir_vector" "CVert3D" "" "" "" "" 3 16 "*-!oVJ;.!}KG5!!") 
                  ("last_start_node" "CBSPNode*" "" "" "" "" 3 16 
                                     "+-!oVJ;.!}KG5!!") 
                  ("last_end_node" "CBSPNode*" "" "" "" "" 3 16 
                                   ",-!oVJ;.!}KG5!!") 
                  ("hit_something" "int" "" "" "" "" 3 16 "--!oVJ;.!}KG5!!") 
                  ("hit_poly_index" "int" "" "" "" "" 3 16 ".-!oVJ;.!}KG5!!") 
                  ("poly_hit_list" "CPolygonList*" "" "" "" "" 3 16 
                                   "/-!oVJ;.!}KG5!!") 
                  ("hit_list_pointer" "CPolygonList*" "" "" "" 
                                      "could point to poly_hit_list or start_node->polygons" 
                                      3 16 "0-!oVJ;.!}KG5!!")) 
                 (("CObject" "" "" 3 16 nil nil "1-!oVJ;.!}KG5!!" "") 
                  ("~CObject" "" "" 3 16 nil nil "2-!oVJ;.!}KG5!!" "") 
                  ("apply_rotation" "void" "" 3 16 
                                    (("dx" "float" "" 0) ("dy" "float" "" 0) 
                                     ("dz" "float" "" 0)) nil 
                                    "3-!oVJ;.!}KG5!!" "") 
                  ("apply_force" "void" "" 3 16 
                                 (("force" "float" "" 0) ("x" "float" "" 0) 
                                  ("y" "float" "" 0) ("z" "float" "" 0)) nil 
                                 "4-!oVJ;.!}KG5!!" "") 
                  ("get_orientation" "CVert3D" "" 3 16 nil nil 
                                     "5-!oVJ;.!}KG5!!" "") 
                  ("set_orientation" "void" "" 3 16 
                                     (("x" "float" "" 0) ("y" "float" "" 0) 
                                      ("z" "float" "" 0)) nil 
                                     "6-!oVJ;.!}KG5!!" "") 
                  ("get_location" "CVert3D" "" 3 16 nil nil "7-!oVJ;.!}KG5!!" 
                                  "") 
                  ("set_location" "void" "" 3 16 
                                  (("x" "float" "" 0) ("y" "float" "" 0) 
                                   ("z" "float" "" 0)) nil "8-!oVJ;.!}KG5!!" 
                                  "") 
                  ("move_object" "int" "" 3 16 
                                 (("unit" "matrix1x4" "" 0) 
                                  ("amount" "float" "" 0) 
                                  ("api" "CRend*" "" 0)) nil "9-!oVJ;.!}KG5!!" 
                                 "") 
                  ("apply_gravity" "void" "" 3 16 (("api" "CRend*" "" 0)) nil 
                                   ":-!oVJ;.!}KG5!!" "") 
                  ("isactive" "int" "" 3 16 nil nil ";-!oVJ;.!}KG5!!" "") 
                  ("activate" "void" "" 3 16 nil nil "<-!oVJ;.!}KG5!!" "") 
                  ("deactivate" "void" "" 3 16 nil nil "=-!oVJ;.!}KG5!!" "") 
                  ("get_dir_vector" "CVert3D" "" 3 16 nil nil 
                                    ">-!oVJ;.!}KG5!!" "") 
                  ("operator=" "inline CObject" "" 3 16 
                               (("o" "CObject" "" 0)) nil "?-!oVJ;.!}KG5!!" 
                               "")) nil nil 10 "") 
         (master "t,!oVJ;.!}KG5!!" 1 4 "CRend" "" "" "" "" "" 1 nil nil nil 
                 nil 10 ""))
(customModel "q,!IWJ;.!}KG5!!" 0 3 "Untitled" "" "" 21 "" "" 1.000000 1.000000 
             (0 0 827 1168) (0 0 827 1168) 
             (place "t,!oVJ;.!}KG5!!" (3) "" 10 "u,!oVJ;.!}KG5!!" 
                    (20 20 72 46) (15 15 77 51) (21 21 71 45) 1 0 
                    (nil 1 -12 32 18 12 18 18) "") 
             (place "v,!oVJ;.!}KG5!!" (3) "" 10 "w,!oVJ;.!}KG5!!" 
                    (100 20 400 588) (95 15 405 593) (104 24 396 584) 1 0 
                    (nil 1 -12 32 18 12 18 18) "") 
             (place "@-!3VJ;.!}KG5!!" (3) "" 10 "A-!3VJ;.!}KG5!!" 
                    (100 639 156 665) (95 634 161 670) (101 640 155 664) 1 0 
                    (nil 1 -12 32 18 12 18 18) "") 
             (place "E-!-VJ;.!}KG5!!" (3) "" 10 "F-!-VJ;.!}KG5!!" 
                    (176 639 216 665) (171 634 221 670) (177 640 215 664) 1 0 
                    (nil 1 -12 32 18 12 18 18) "") 
             (place "J-!(VJ;.!}KG5!!" (3) "" 10 "K-!(VJ;.!}KG5!!" 
                    (243 639 325 665) (238 634 330 670) (244 640 324 664) 1 0 
                    (nil 1 -12 32 18 12 18 18) "") 
             (place "O-!nUJ;.!}KG5!!" (3) "" 10 "P-!nUJ;.!}KG5!!" 
                    (350 639 402 665) (345 634 407 670) (351 640 401 664) 1 0 
                    (nil 1 -12 32 18 12 18 18) "") 
             (link "A-!3VJ;.!}KG5!!" "w,!oVJ;.!}KG5!!" 
                   (124 639 124 616 252 616 252 587) 3 "" "" "%%" "%%" "" "" 3 
                   1 (179 609 197 623) (120 631 120 631) (248 595 248 595) 0 0 
                   "T-!GUJ;.!}KG5!!" (124 587 252 639) (109 582 267 644) 
                   (178 608 198 624) 2 0 (nil 1 -12 32 18 12 18 18) "") 
             (link "F-!-VJ;.!}KG5!!" "w,!oVJ;.!}KG5!!" 
                   (192 639 192 616 252 616 252 587) 3 "" "" "%%" "%%" "" "" 3 
                   1 (213 609 231 623) (188 631 188 631) (248 595 248 595) 0 0 
                   "U-!CUJ;.!}KG5!!" (192 587 252 639) (177 582 267 644) 
                   (212 608 232 624) 2 0 (nil 1 -12 32 18 12 18 18) "") 
             (link "K-!(VJ;.!}KG5!!" "w,!oVJ;.!}KG5!!" (252 639 252 587) 3 "" 
                   "" "%%" "%%" "" "" 3 0 (243 606 261 620) (248 631 248 631) 
                   (248 595 248 595) 0 0 "V-!@UJ;.!}KG5!!" (242 605 262 621) 
                   (237 582 267 644) (242 605 262 621) 2 0 
                   (nil 1 -12 32 18 12 18 18) "") 
             (link "P-!nUJ;.!}KG5!!" "w,!oVJ;.!}KG5!!" 
                   (364 639 364 616 252 616 252 587) 3 "" "" "%%" "%%" "" "" 
                   3 1 (299 609 317 623) (360 631 360 631) (248 595 248 595) 
                   0 0 "W-!;UJ;.!}KG5!!" (252 587 364 639) (237 582 379 644) 
                   (298 608 318 624) 2 0 (nil 1 -12 32 18 12 18 18) ""))
