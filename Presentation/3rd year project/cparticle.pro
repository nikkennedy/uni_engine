(UMLStudio "6.2" project)
(repository "" 0)
(notation "UML.not")
(genProfile 178 1 ("" "" "" "" "") "" 0 ("" "" "" "" "") ("" "" "" "" "") 700 
            360 201 441 70 50 80 50 80 60 0 0)
(codeFiles)
(docFiles)
(otherFiles)
(revFiles 1)
(masters (master "C'!=BV;.!}KG5!!" 1 3 "COpenGLRend" "" "" "" "" "" 1 nil 
                 (("add_particle_system" "int" "" 3 16 
                                         (("ps" "CParticleSystem *" "" 1)) 
                                         nil "U'!d@V;.!}KG5!!" "")) nil nil 10 
                 "") 
         (master "v&!3FV;.!}KG5!!" 1 3 "CRend" "" "" "" "" "" 1 nil 
                 (("add_particle_system" "int" "" 3 16 
                                         (("ps" "CParticleSystem *" "" 1)) 
                                         nil "N'!NAV;.!}KG5!!" "")) nil nil 10 
                 "") 
         (master "P&!'HV;.!}KG5!!" 1 3 "CParticle" "" "" "" "" "" 1 
                 (("position" "CVert3D" "" "" "" "" 1 16 "V&!8GV;.!}KG5!!") 
                  ("velocity" "CVert3D" "" "" "" "" 1 16 "W&!4GV;.!}KG5!!") 
                  ("acceleration" "CVert3D" "" "" "" "" 1 16 "X&!/GV;.!}KG5!!") 
                  ("age" "int" "" "" "" "" 1 16 "Y&!,GV;.!}KG5!!") 
                  ("max_age" "int" "" "" "" "" 1 16 "Z&!+GV;.!}KG5!!") 
                  ("size" "float" "" "" "" "" 1 16 "[&!~FV;.!}KG5!!") 
                  ("alive" "int" "" "" "" "" 1 16 "\&!|FV;.!}KG5!!")) nil nil 
                 nil 10 "") 
         (master "H&!kHV;.!}KG5!!" 1 3 "COGLParticleSystem" "" "" "" "" "" 1 
                 nil 
                 (("render" "void" "" 3 16 nil nil "K&!9HV;.!}KG5!!" "") 
                  ("COGLParticleSystem" "" "" 3 16 
                                        (("ps" "CParticleSystem *" "" 1) 
                                         ("tm" "COGLTextureManager *" "" 1)) 
                                        nil "$'!9EV;.!}KG5!!" "")) nil nil 10 
                 "") 
         (master "4&!KJV;.!}KG5!!" 1 3 "CParticleSystem" "" "" "" "" "" 1 nil 
                 (("reset_system" "void" "" 3 16 nil nil "7&!6JV;.!}KG5!!" "") 
                  ("set_particle_defaults" "void" "" 3 16 (("i" "int" "" 1)) 
                                           nil "9&!#JV;.!}KG5!!" "") 
                  ("update_system" "int" "" 3 16 
                                   (("time_passed" "float" "" 1)) nil 
                                   ";&!nIV;.!}KG5!!" "") 
                  ("render" "void" "" 3 16 nil nil "=&!`IV;.!}KG5!!" "") 
                  ("CParticleSystem" "" "" 3 16 
                                     (("num_particles" "int" "" 1) 
                                      ("origin" "CVert3D" "" 1) 
                                      ("pt_filename" "char *" "" 1)) nil 
                                     "2'!SDV;.!}KG5!!" "")) nil nil 10 ""))
(customModel "3&![JV;.!}KG5!!" 0 3 "Untitled" "" "" 21 "" "" 1.000000 1.000000 
             (0 0 827 1168) (0 0 827 1168) 
             (place "4&!KJV;.!}KG5!!" (3) "" 10 "5&!KJV;.!}KG5!!" 
                    (232 45 592 147) (227 40 597 152) (236 47 588 145) 1 0 
                    (nil 1 -12 32 18 12 18 18) "") 
             (place "H&!kHV;.!}KG5!!" (3) "" 10 "I&!kHV;.!}KG5!!" 
                    (273 183 639 241) (268 178 644 246) (277 184 635 239) 1 0 
                    (nil 1 -12 32 18 12 18 18) "") 
             (place "P&!'HV;.!}KG5!!" (3) "" 10 "Q&!'HV;.!}KG5!!" 
                    (15 47 137 177) (10 42 142 182) (16 49 136 175) 1 0 
                    (nil 1 -12 32 18 12 18 18) "") 
             (link "I&!kHV;.!}KG5!!" "5&!KJV;.!}KG5!!" (456 183 456 146) 3 "" 
                   "" "%%" "%%" "" "" 3 0 (447 157 465 171) (452 175 452 175) 
                   (452 154 452 154) 0 0 "q&! FV;.!}KG5!!" (446 156 466 172) 
                   (441 141 471 188) (446 156 466 172) 2 0 
                   (nil 1 -12 32 18 12 18 18) "") 
             (link "5&!KJV;.!}KG5!!" "Q&!'HV;.!}KG5!!" (232 86 136 86) 3 
                   "particles" "" "%%" "%%" "" "" 3 0 (154 77 214 95) 
                   (202 67 224 82) (144 67 166 82) 4 4 "r&!KFV;.!}KG5!!" 
                   (154 77 214 95) (131 62 237 106) (154 77 214 95) 2 0 
                   (nil 1 -12 32 18 12 18 18) "") 
             (place "v&!3FV;.!}KG5!!" (3) "" 10 "w&!3FV;.!}KG5!!" 
                    (13 194 259 238) (8 189 264 243) (16 195 256 237) 1 0 
                    (nil 1 -12 32 18 12 18 18) "") 
             (place "C'!=BV;.!}KG5!!" (3) "" 10 "D'!=BV;.!}KG5!!" 
                    (13 290 259 334) (8 285 264 339) (16 291 256 333) 1 0 
                    (nil 1 -12 32 18 12 18 18) "") 
             (link "I&!kHV;.!}KG5!!" "D'!=BV;.!}KG5!!" 
                   (476 240 476 312 258 312) 3 "particlesystems" "" "%%" "%%" 
                   "" "" 3 1 (315 303 419 321) (450 248 472 263) 
                   (266 293 288 308) 4 4 "H'!!BV;.!}KG5!!" (258 240 476 321) 
                   (253 235 491 332) (315 303 419 321) 2 0 
                   (nil 1 -12 32 18 12 18 18) "") 
             (link "D'!=BV;.!}KG5!!" "w&!3FV;.!}KG5!!" (132 290 132 238) 3 "" 
                   "" "%%" "%%" "" "" 3 0 (123 257 141 271) (128 282 128 282) 
                   (128 246 128 246) 0 0 "Z'!Q@V;.!}KG5!!" (122 256 142 272) 
                   (117 233 147 295) (122 256 142 272) 2 0 
                   (nil 1 -12 32 18 12 18 18) ""))
