module Main where

import Data.List (sort, sortBy)

data SegmentTree a = Node a (SegmentTree a) (SegmentTree a) | Empty deriving (Show)

data SegTree a = Seg
  { len :: Int,
    tree :: SegmentTree a
  }
  deriving (Show)

data Task a = Task
  { stree :: [SegTree a],
    vec :: [a],
    taskLen :: Int
  }
  deriving (Show)

-- it could be more generic but i dont like this dsa enough to do it
fmapTree (Node i left right) tl tr l r
  | l == tl && r == tr = i
  | otherwise = leftS + rightS
  where
    leftS = if l <= tm then fmapTree left tl tm l (min r tm) else 0
    rightS = if r >= tm + 1 then fmapTree right (tm + 1) tr (max l (tm + 1)) r else 0
    tm = div (tl + tr) 2

makeNodeAlive (Node i left right) tl tr l r
  | l == tl && r == tr = Node (i + 1) left right
  | otherwise = Node (i + 1) leftAliving rightAliving
  where
    tm = div (tl + tr) 2
    leftAliving = if l <= tm then makeNodeAlive left tl tm l (min r tm) else left
    rightAliving = if r >= tm + 1 then makeNodeAlive right (tm + 1) tr (max l (tm + 1)) r else right

binSearch vec el = go ls rs
  where
    ls = 0
    rs = length vec
    go l r
      | l < r - 1 = go newl newr
      | otherwise = r
      where
        m = div (l + r) 2
        cur = vec !! m
        newl = if cur >= el then m else l
        newr = if cur >= el then r else m

build vec tl tr
  | tl == tr = Node (vec !! tl) Empty Empty
  | otherwise = Node (il + ir) leftBuild rightBuild
  where
    tm = div (tl + tr) 2
    leftBuild = build vec tl tm
    rightBuild = build vec (tm + 1) tr
    (Node il _ _) = leftBuild
    (Node ir _ _) = rightBuild

buildForTask vec tl tr
  | tl == tr = Node 0 Empty Empty
  | otherwise = Node 0 leftBuild rightBuild
  where
    tm = div (tl + tr) 2
    leftBuild = buildForTask vec tl tm
    rightBuild = buildForTask vec (tm + 1) tr

sumRange (Seg ln stree) = fmapTree stree 0 ln

alivePoint (Seg ln stree) p = Seg {tree = makeNodeAlive stree 0 ln p p, len = ln}

addIndicesToList xs = go xs 0
  where
    go [] _ = []
    go (x : xs) i = (x, i) : go xs (i + 1)

makePersistentTreesList vec = Task {stree = go sorted [segTree], vec = map fst sorted, taskLen = ln + 1}
  where
    sorted = sortBy (\(a, _) (b, _) -> compare b a) $ addIndicesToList vec
    stree = buildForTask vec 0 ln
    ln = length vec - 1
    segTree = Seg {tree = stree, len = ln}
    go [] acc = reverse acc
    go ((_, p) : xs) (a : acc) = go xs (aliveTree : a : acc)
      where
        aliveTree = alivePoint a p

moreEqualKRange (Task streeList vec taskLen) k l r = ans
  where
    n = binSearch vec k
    tree = streeList !! n
    ans = sumRange tree l r

a = [46, 11, 40, 8, 2, 42, 65, 10]

ptl = makePersistentTreesList a

main :: IO ()
main = do
  print $ moreEqualKRange ptl 42 0 7
