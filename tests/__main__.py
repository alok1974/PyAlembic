import unittest
from IexTest import IexTest
from ImathTest import ImathTest

if __name__ == '__main__':
    unittest.TextTestRunner(IexTest)
    unittest.TextTestRunner(verbosity=2).run(ImathTest)

