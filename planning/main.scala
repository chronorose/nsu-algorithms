import scala.io.Source
import scala.util.Sorting
import scala.collection.mutable.ArrayBuffer
import scala.collection.mutable.HashMap

class Task(val deadline: Int, val weight: Int):
  override def toString(): String =
    s"deadline: $deadline, weight: $weight"
  

object TaskCompare extends Ordering[Task]:
  def compare(a: Task, b: Task) = b.weight.compare(a.weight)

class UnionFind(size: Int):
  var arr: Array[Int] = new Array(size)
  var ranks: Array[Int] = new Array(size)
  var sizes: Array[Int] = new Array(size) 
  var penalty: Int = 0
  for i <- 0 until size do
    arr(i) = i
    ranks(i) = 0
    sizes(i) = 0
        
  def union(eq1: Int, eq2: Int): Unit =
    val c1 = find(eq1)
    val c2 = find(eq2) 
    if c1 == c2 then return
    if ranks(c1) > ranks(c2) then
      sizes(c1) += sizes(c2)
      arr(c2) = c1 
    else
      sizes(c2) += sizes(c1) 
      arr(c1) = c2
      if ranks(c1) == ranks(c2) then
        ranks(c2) += 1

  def find(eq: Int): Int =
    if arr(eq) == eq then
      eq
    else
      arr(eq) = find(arr(eq))
      arr(eq)

  def find_place(task: Task) =
    var index = task.deadline
    while index >= 0 && (sizes(index) != 0) do
      index -= sizes(index) 
    if (index < 0) then
      index = (size - 1) - sizes(size - 1)
      sizes(index) += 1
      penalty += task.weight
      if index != size - 1 then
        union(index, size - 1)
    else
      sizes(index) += 1
      if index != task.deadline then
        union(index, task.deadline)
    index

  override def toString(): String = 
    var str = StringBuilder()
    for i <- 0 until arr.size do
      val c = arr(i)
      val size = sizes(i)
      str ++= s"i: $i, c: $c, size: $size\n" 
    str += '\n'
    str.toString()

def parseTest(path: String) =
  var tasks: ArrayBuffer[Task] = ArrayBuffer()
  val lines = io.Source.fromFile(path).getLines().toList
  val first_line = lines.head
  var rest = lines.tail
  val amount = first_line.trim().toInt
  for i <- 0 until amount do
    val Array(deadline, weight) = rest.head.split(" ").map(x => x.toInt) 
    tasks.append(Task(deadline - 1, weight))
    rest = rest.tail
  tasks

@main def m(args: String*) =
  val tasks = parseTest(args(0))
  val uf = UnionFind(tasks.size)
  tasks.sortInPlace()(TaskCompare)
  var map: HashMap[Task, Int] = HashMap()
  for i <- 0 until tasks.size do
    map.addOne(tasks(i), i)
  for i <- tasks do
    val new_index = uf.find_place(i)
    map(i) = new_index
  var out: Array[Task] = new Array(tasks.size)
  for (task, i) <- map do
    out(i) = task 

  out.zipWithIndex.foreach(
    (task, i) => 
      println(s"task with weight ${task.weight} and deadline ${task.deadline + 1} will be done at day ${i + 1}"))
  println(s"overall penalty: ${uf.penalty}")
