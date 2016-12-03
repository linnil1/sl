import slpyc
def sl(cols,lines,arg=''):
    slpyc.init(cols,lines,arg)
    while True:
        x = slpyc.step()
        if x:
            yield x
        else:
            return None
