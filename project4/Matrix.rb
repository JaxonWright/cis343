#
#  Class that represents a Matrix and implements operations on matrices.
#
#  Author(s): Chad Teitsma and Jaxon Wright
#
class Matrix

  # create getter methods for instance variables @rows and @columns
  attr_reader  :rows, :columns

  # create setter methods for instance variables @rows and @columns
  attr_writer  :rows, :columns

  # make setter methods for @rows and @columns private
  private :rows=, :columns=
  
  # method that initializes a newly allocated Matrix object
  # use instance variable named @data (an array) to hold matrix elements
  # raise ArgumentError exception if any of the following is true:
  #     parameters rows or columns or val is not of type Fixnum
  #     value of rows or columns is <= 0
  def initialize(rows=5, columns=5, val=0)
    if(rows < 1 || columns < 1) ||
      (rows.class != Fixnum || columns.class != Fixnum || val.class != Fixnum)
        raise ArgumentError.new("Rows and Columns must be > 0. Only numbers are allowed")
    end
    self.rows=(rows)
    self.columns=(columns)
    @data = Array.new(rows*columns,val)
  end

  # method that returns matrix element at location (i,j)
  # NOTE: row and column values are zero-index based
  # raise ArgumentError exception if any of the following is true:
  #     parameters i or j is not of type Fixnum
  #     value of i or j is outside the bounds of Matrix
  def get(i, j)
    raise ArgumentError.new("i and j must be numbers") unless i.class == Fixnum && j.class == Fixnum
    raise ArgumentError.new("outside of matrix bounds") unless i<@rows && j<@columns && i>=0 && j>=0
    
    return @data[i*@columns + j]
  end

  # method to set the value of matrix element at location (i,j) to value of parameter val
  # NOTE: row and column values are zero-index based
  # raise ArgumentError exception if any of the following is true:
  #     parameters i or j or val is not of type Fixnum
  #     the value of i or j is outside the bounds of Matrix
  def set(i, j, val)
    raise ArgumentError.new("i and j must be numbers") unless i.class == Fixnum && j.class == Fixnum
    raise ArgumentError.new("outside of matrix bounds") unless i<@rows && j<@columns && i>=0 && j>=0

    @data[i*@columns + j] = val
  end

  # method that returns a new matrix object that is the sum of this and parameter matrices.
  # raise ArgumentError exception if the parameter m is not of type Matrix
  # raise IncompatibleMatricesError exception if the matrices are not compatible for addition operation
  def add(m)
    raise ArgumentError.new("must pass in a matrix") unless m.class == Matrix
    raise IncompatibleMatricesError.new("matrices must be the same size to add") unless m.rows == @rows && m.columns == @columns
    
    added = Matrix.new(@rows,@columns)
    for i in 0...@rows
      for j in 0...@columns
        added.set(i,j,self.get(i,j)+m.get(i,j))
      end
    end
    return added
  end

  # method that returns a new matrix object that is the difference of this and parameter matrices
  # raise ArgumentError exception if the parameter m is not of type Matrix
  # raise IncompatibleMatricesError exception if the matrices are not compatible for subtraction operation
  def subtract(m)
    raise ArgumentError.new("must pass in a matrix") unless m.class == Matrix
    raise IncompatibleMatricesError.new("matrices must be the same size to subtract") unless m.rows == @rows && m.columns == @columns
    
    subbed = Matrix.new(@rows,@columns)
    for i in 0...@rows
      for j in 0...@columns
        subbed.set(i,j,self.get(i,j)-m.get(i,j))
      end
    end
    return subbed
  end

  # method that returns a new matrix object that is a scalar multiple of source matrix object
  # raise ArgumentError exception if the parameter k is not of type Fixnum
  def scalarmult(k)
    raise ArgumentError.new("must pass in a number") unless k.class == Fixnum
    
    scalared = Matrix.new(@rows,@columns)
    for i in 0...@rows
      for j in 0...@columns
        scalared.set(i,j,self.get(i,j)*k)
      end
    end
    return scalared
  end

  # method that returns a new matrix object that is the product of this and parameter matrices
  # raise ArgumentError exception if the parameter m is not of type Matrix
  # raise IncompatibleMatricesError exception if the matrices are not compatible for multiplication operation
  def multiply(m)
    raise ArgumentError.new("must pass in a matrix") unless m.class == Matrix
    raise IncompatibleMatricesError.new("rows and columns incompatible") unless @columns == m.rows
    
    multiplied = Matrix.new(@rows,m.columns)
    temp = 0
    for i in 0...@rows
      for j in 0...m.columns
        for k in 0...@columns
          temp += self.get(i,k) * m.get(k,j)
        end
        multiplied.set(i,j,temp)
        temp = 0
      end
    end
    return multiplied
  end

  # method that returns a new matrix object that is the transpose of the source matrix
  def transpose
    transposed = Matrix.new(@columns,@rows)
    for i in 0...transposed.rows
      for j in 0...transposed.columns
        transposed.set(i,j,self.get(j,i))
      end
    end
    return transposed
  end

  # overload + for matrix addition
  def +(m)
    add(m)
  end

  # overload - for matrix subtraction
  def -(m)
    subtract(m)
  end

  # overload * for matrix multiplication
  def *(m)
    multiply(m)
  end

  # class method that returns an identity matrix with size number of rows and columns
  # raise ArgumentError exception if any of the following is true:
  #     parameter size is not of type Fixnum
  #     the value of size <= 0
  def Matrix.identity(size)
    raise ArgumentError.new("must pass in a number > 0") unless size.class == Fixnum && size > 0
    
    iden = Matrix.new(size,size)
    for i in 0...iden.rows
      for j in 0...iden.columns
        if i == j
          iden.set(i,j,1)
        end
      end
    end
    return iden
  end

  # method that sets every element in the matrix to value of parameter val
  # raise ArgumentError exception if val is not of type Fixnum
  # hint: use fill() method of Array to fill the matrix
  def fill(val)
    raise ArgumentError.new("must pass in a number") unless val.class == Fixnum
    
    @data.fill(val)
  end

  # method that return a deep copy/clone of this matrix object
  def clone
    cloned = Matrix.new(@rows,@columns)
    
    for i in 0...cloned.rows
      for j in 0...cloned.columns
        cloned.set(i,j,self.get(i,j))
      end
    end
    return cloned
  end

  # method that returns true if this matrix object and the parameter matrix object are equal
  # (i.e., have the same number of rows, columns, and corresponding values in the two
  # matrices are equal). Otherwise, it returns false.
  # returns false if the parameter m is not of type Matrix
  def ==(m)
    if m.class != Matrix || @rows != m.rows || @columns != m.columns
      return false
    end
    
    for i in 0...@rows
      for j in 0...@columns
        if self.get(i,j) != m.get(i,j)
          return false
        end
      end
    end
    return true
  end

  # method that returns a string representation of matrix data in table (row x col) format
  def to_s
    str = ""
    for i in 0...@rows
      for j in 0...@columns
        str << get(i,j).to_s
        if j != @columns - 1
          str << " "
        end
      end
      str << "\n"
    end
    str << "\n"
    return str
  end

  # method that for each element in the matrix yields with information
  # on row, column, and data value at location (i,j)
  def each
    for i in 0...@rows
      for j in 0...@columns
        yield(i,j,get(i,j))
      end
    end
  end
  
end

#
# Custom exception class IncompatibleMatricesError
#
class IncompatibleMatricesError < Exception
  def initialize(msg)
    super msg
  end
end

#
#  main test driver
#
def main
  m1 = Matrix.new(3,4,10)
  m2 = Matrix.new(3,4,20)
  m3 = Matrix.new(4,5,30)
  m4 = Matrix.new(3,5,40)

  puts(m1)
  puts(m2)
  puts(m3)
  puts(m4)

  puts(m1.add(m2))

  puts(m1.subtract(m2))

  puts(m1.multiply(m3))

  puts(m2.scalarmult(5))

  puts(Matrix.identity(5))

  puts(m1 + m2)

  puts(m2 - m1)

  puts(m1 * m3)

  puts(m1 + m2 - m1)

  puts(m4 + m2 * m3)

  puts(m1.clone())

  puts(m1.transpose())

  puts("Are matrices equal? #{m1 == m2}")

  puts("Are matrices equal? #{m1 == m3}")

  puts("Are matrices equal? #{m1 == m1}")

  m1.each { |i, j, val|
    puts("(#{i},#{j},#{val})")
  }

  begin
    m1.get(4,4)
  rescue ArgumentError => exp
    puts("#{exp.message} - get failed\n")
  end

  begin
    m1.set(4,5,10)
  rescue ArgumentError => exp
    puts("#{exp.message} - set failed\n")
  end

  begin
    m1.add(m3)
  rescue IncompatibleMatricesError => exp
    puts("#{exp.message} - add failed\n")
  end

  begin
    m2.subtract(m3)
  rescue IncompatibleMatricesError => exp
    puts("#{exp.message} - subtract failed\n")
  end

  begin
    m1.multiply(m2)
  rescue IncompatibleMatricesError => exp
    puts("#{exp.message} - multiply failed\n")
  end

  begin
    m1 + m3
  rescue IncompatibleMatricesError => exp
    puts("#{exp.message} - add failed\n")
  end

  begin
    m2 - m3
  rescue IncompatibleMatricesError => exp
    puts("#{exp.message} - subtract failed\n")
  end

  begin
    m1 * m2
  rescue IncompatibleMatricesError => exp
    puts("#{exp.message} - multiply failed\n")
  end

end

# uncomment the following line to run the main() method
#main()