



// apply f for all elements thats suffisate t
// 3 i have no idea
def g[A](l: List[A], t: (A) => Bool, f: (A) => A): List[A] =
    
    def rec(l, t, f, result) {
        l match 
            case Nil => result
            case l_1 : l_other =>
                if t(l_1)
                    rec(l_other, t, f, f(l_1) : result)
                else 
                    rec(l_other, t, f, result)

    }
    
    rec.reversed()


def head[A](l: MyList[A]) => 


