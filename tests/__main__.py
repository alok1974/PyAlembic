import unittest
from IexTest import IexTest
from ImathTest import ImathTest

if __name__ == '__main__':
    suite = unittest.TestLoader().loadTestsFromTestCase(IexTest)
    suite.addTest(ImathTest)
    unittest.TextTestRunner(verbosity=2).run(suite)

