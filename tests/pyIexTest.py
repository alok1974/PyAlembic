import sys
import os

# for iex.pyd
root=os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.append(os.path.join(root, 'build/lib.win-amd64-2.7'))

# for Iex-2_2.dll
dll_dir='{0}\\installed\\x64-windows\\bin;'.format(os.environ['VCPKG_DIR'])
os.environ['PATH']=dll_dir+';'+os.environ['PATH']

import unittest
import iex


def raise_class(klass, what):
    def _raise_class():
        raise klass(what)
    return _raise_class


class Test(unittest.TestCase):

    def test_BaseExc(self):
        e = iex.BaseExc('test')
        self.assertTrue(isinstance(e, iex.BaseExc))
        self.assertTrue(isinstance(e, Exception))

    def test_raise(self):
        self.assertRaises(iex.BaseExc, raise_class(iex.BaseExc, 'test'))
        self.assertRaises(Exception, raise_class(iex.EpermExc, 'test'))

    def test_raise_in_cpp(self):
        #self.assertRaises(Exception, iex.testCxxExceptions,  0)
        self.assertRaises(Exception, iex.testCxxExceptions, 1)
        self.assertRaises(Exception, iex.testCxxExceptions, 2)
        self.assertRaises(iex.BaseExc, iex.testCxxExceptions, 3)
        self.assertRaises(iex.ArgExc, iex.testCxxExceptions, 4)

    def test_raise(self):
        self.assertRaises(iex.BaseExc, raise_class(iex.BaseExc, 'new BaseExc from python'))
        self.assertRaises(iex.ArgExc, raise_class(iex.ArgExc, 'new ArgExc from python'))
        self.assertRaises(iex.BaseExc, raise_class(iex.ArgExc, 'new ArgExc from python'))

    def test_conversion(self):
        #be = iex.testMakeBaseExc('testStr')
        be=iex.BaseExc('testStr')
        self.assertTrue(isinstance(be,iex.BaseExc))
        self.assertTrue(be.__class__ == iex.BaseExc)

        #ae = iex.testMakeArgExc('testStr')
        ae = iex.ArgExc('testStr')
        self.assertTrue(isinstance(ae,iex.BaseExc))
        self.assertTrue(isinstance(ae,iex.ArgExc))
        self.assertTrue(ae.__class__ == iex.ArgExc)

        #self.assertTrue(iex.testBaseExcString(be) == 'testStr')
        #self.assertTrue(iex.testBaseExcString(ae) == 'testStr')
        #self.assertTrue(iex.testArgExcString(ae) == 'testStr')


if __name__ == '__main__':
    unittest.main()
