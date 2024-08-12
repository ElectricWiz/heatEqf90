program heat1d
    implicit none

    integer, parameter :: Nx = 24
    integer :: i, t

    real, parameter :: s = 0.1
    real, parameter :: deltaX = 1.0/(Nx - 1)
    
    real, parameter :: deltaT = s * (deltaX*deltaX)/0.5
    real :: Nt = floor(1/deltaT)

    real, dimension(Nx) :: uj_1, duj_1, d2uj_1
    real, dimension(Nx) :: x

    open(unit=10, file='heat_data.csv', status='replace')

    call random_seed()
    
    ! Initialize uj_1 with 0.125 and x with position values
    do i = 1, Nx
        uj_1(i) = 0.0
        x(i) = (i - 1) * deltaX
    end do

    ! Assign specific values to certain elements
    uj_1(floor(real(Nx)/2.0)) = 1.0
    uj_1(Nx) = 0.0
    uj_1(1) = 0.0

    ! Write header to CSV file
    write(10, '(A)', advance='no') 'Time'
    do i = 1, Nx
        write(10, '(A, F6.2)', advance='no') ',', x(i)
    end do
    write(10, *)

    ! Write initial conditions to CSV file
    write(10, '(I0)', advance='no') 0
    do i = 1, Nx
        write(10, '(A, F6.2)', advance='no') ',', uj_1(i)
    end do
    write(10, *)

    do t = 1, min(int(Nt), 300)
        ! Calculate the second spatial derivative using central difference
        do i = 2, Nx-1
            d2uj_1(i) = (uj_1(i+1) - 2.0*uj_1(i) + uj_1(i-1)) / (deltaX*deltaX)
        end do

        ! Update the temperature distribution for the next time step
        do i = 2, Nx-1
            duj_1(i) = uj_1(i) + deltaT * d2uj_1(i)
        end do

        ! Apply boundary conditions
        duj_1(1) = 0.0
        duj_1(Nx) = 0.0

        ! Update uj_1 for the next iteration
        uj_1 = duj_1

        ! Write data every 5 frames
        if (mod(t, 5) == 0) then
            write(10, '(I0)', advance='no') t
            do i = 1, Nx
                write(10, '(A, F6.2)', advance='no') ',', uj_1(i)
            end do
            write(10, *)
        end if
    end do

    close(10)
end program heat1d
