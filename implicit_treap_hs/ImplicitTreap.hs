module ImplicitTreap where

import System.Random hiding (split)

type Priority = Int

type Val = Int

type Sum = Int

type Count = Int

data Treap = Node Val Priority Sum Count Treap Treap | Empty

singleton v p = Node v p v 1 Empty Empty

size Empty = 0
size (Node _ _ _ _ l r) = 1 + size l + size r

inplaceSize Empty = 0
inplaceSize (Node _ _ _ c _ _) = c

sumT Empty = 0
sumT (Node v _ _ _ l r) = v + sumT l + sumT r

rebuildNode v p l r = updateNode (Node v p 0 1 l r)

-- тут неудобная дубликация кода получается(я по сути переписываю и sumT и size снова, но как будто от неё никак не избавиться)
updateNode (Node v p _ _ l r) = Node v p (sumT l + sumT r + v) (1 + size l + size r) l r

splitBySize Empty _ = (Empty, Empty)
splitBySize (Node v p _ _ l r) k
  | k <= inplaceSize l = (ll, tr)
  | otherwise = (tl, rr)
  where
    (ll, lr) = splitBySize l k
    (rl, rr) = splitBySize r (k - inplaceSize l - 1)
    tr = rebuildNode v p lr r
    tl = rebuildNode v p l rl

merge t1 t2 =
  case (t1, t2) of
    (Empty, t2) -> t2
    (t1, Empty) -> t1
    (t1, t2) -> mergeHelper t1 t2
  where
    mergeHelper (Node v1 p1 c1 s1 l1 r1) (Node v2 p2 c2 s2 l2 r2)
      | p1 < p2 = Node v1 p1 c1 s1 l1 (merge r1 t2)
      | otherwise = Node v2 p2 c2 s2 (merge t1 l2) r2

-- so it works perfectly, the only trouble is that it starts from 1 not from 0(i guess it's alright)
sumImpl t from to = s
  where
    (_, r) = splitBySize t (from - 1)
    (Node _ _ s _ _ _, _) = splitBySize r (to - from + 1)

insert t v p pos = merge l $ merge (Node v p 1 v Empty Empty) r
  where
    (l, r) = splitBySize t pos

erase t pos = merge l rr
  where
    (l, r) = splitBySize t (pos - 1)
    (_, rr) = splitBySize r 1

eraseMult t pos c = merge l rr
  where
    (l, r) = splitBySize t (pos - 1)
    (_, rr) = splitBySize r c

toListT Empty = []
toListT (Node v p _ _ l r) = toListT l ++ ((v, p) : toListT r)

fromListT xs =
  let helper ((v, p) : xs) t c = helper xs (insert t v p c) (c + 1)
      helper [] t _ = t
   in helper xs Empty 0

xs = [(30, 4), (16, 8), (18, 7), (23, 6), (17, 8), (226, 7), (1, 13)]

randomList num seed = take num (zip xs ys)
  where
    xs = randoms $ mkStdGen seed :: [Int]
    ys = randoms $ mkStdGen (seed + 123) :: [Int]

test = aux (cap - 1) (cap - 1)
  where
    cap = 1000
    xs = randomList cap 10
    t = fromListT xs
    aux from to = sumImpl t (from + 1) (to + 1) == cnt (map fst xs) from to
    aux2 c1 c2
      | c2 <= 1 = aux2 (c1 - 1) cap
      | c1 <= 1 = True
      | otherwise = aux2 c1 (c2 - 1) && aux c1 c2

cnt xs from to =
  let helper (x : xs) from to c
        | from > c = helper xs from to (c + 1)
        | from < to = x + helper xs (from + 1) to (c + 1)
        | otherwise = x
      helper [] from to c = 0
   in helper xs from to 0

main = do
  print $ (toListT . fromListT) xs
  print $ toListT (insert (fromListT xs) 10 130 0)
  print $ toListT (erase (fromListT xs) 2)
  print $ toListT (eraseMult (fromListT xs) 1 4)
  print test
