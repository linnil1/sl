import slpyc
def sl(cols,lines,arg=''):
    """
    arg 
      -r random flags
      -d add dance people
      -l add more locomotives 
         (number of l = number of loco)
      -F Fly
      -c 
      -a add people cry for help
    """
      
    slpyc.init(cols,lines,arg)
    while True:
        x = slpyc.step()
        if x:
            yield x
        else:
            return None
