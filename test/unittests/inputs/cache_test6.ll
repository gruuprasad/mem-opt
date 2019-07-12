; ModuleID = 'cache_test6.c'
source_filename = "cache_test6.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%struct.pl_memory = type { [32 x %struct.packet] }
%struct.packet = type { i64, i64, i64, i64 }

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local void @test_fn(%struct.pl_memory**) #0 {
  %2 = alloca %struct.pl_memory**, align 8
  store %struct.pl_memory** %0, %struct.pl_memory*** %2, align 8
  %3 = load %struct.pl_memory**, %struct.pl_memory*** %2, align 8
  %4 = getelementptr inbounds %struct.pl_memory*, %struct.pl_memory** %3, i64 0
  %5 = load %struct.pl_memory*, %struct.pl_memory** %4, align 8
  %6 = getelementptr inbounds %struct.pl_memory, %struct.pl_memory* %5, i32 0, i32 0
  %7 = getelementptr inbounds [32 x %struct.packet], [32 x %struct.packet]* %6, i64 0, i64 1
  %8 = getelementptr inbounds %struct.packet, %struct.packet* %7, i32 0, i32 0
  store i64 3, i64* %8, align 8
  %9 = load %struct.pl_memory**, %struct.pl_memory*** %2, align 8
  %10 = getelementptr inbounds %struct.pl_memory*, %struct.pl_memory** %9, i64 0
  %11 = load %struct.pl_memory*, %struct.pl_memory** %10, align 8
  %12 = getelementptr inbounds %struct.pl_memory, %struct.pl_memory* %11, i32 0, i32 0
  %13 = getelementptr inbounds [32 x %struct.packet], [32 x %struct.packet]* %12, i64 0, i64 1
  %14 = getelementptr inbounds %struct.packet, %struct.packet* %13, i32 0, i32 1
  store i64 2, i64* %14, align 8
  %15 = load %struct.pl_memory**, %struct.pl_memory*** %2, align 8
  %16 = getelementptr inbounds %struct.pl_memory*, %struct.pl_memory** %15, i64 0
  %17 = load %struct.pl_memory*, %struct.pl_memory** %16, align 8
  %18 = getelementptr inbounds %struct.pl_memory, %struct.pl_memory* %17, i32 0, i32 0
  %19 = getelementptr inbounds [32 x %struct.packet], [32 x %struct.packet]* %18, i64 0, i64 1
  %20 = getelementptr inbounds %struct.packet, %struct.packet* %19, i32 0, i32 2
  store i64 4, i64* %20, align 8
  %21 = load %struct.pl_memory**, %struct.pl_memory*** %2, align 8
  %22 = getelementptr inbounds %struct.pl_memory*, %struct.pl_memory** %21, i64 0
  %23 = load %struct.pl_memory*, %struct.pl_memory** %22, align 8
  %24 = getelementptr inbounds %struct.pl_memory, %struct.pl_memory* %23, i32 0, i32 0
  %25 = getelementptr inbounds [32 x %struct.packet], [32 x %struct.packet]* %24, i64 0, i64 1
  %26 = getelementptr inbounds %struct.packet, %struct.packet* %25, i32 0, i32 3
  store i64 1, i64* %26, align 8
  %27 = load %struct.pl_memory**, %struct.pl_memory*** %2, align 8
  %28 = getelementptr inbounds %struct.pl_memory*, %struct.pl_memory** %27, i64 1
  %29 = load %struct.pl_memory*, %struct.pl_memory** %28, align 8
  %30 = getelementptr inbounds %struct.pl_memory, %struct.pl_memory* %29, i32 0, i32 0
  %31 = getelementptr inbounds [32 x %struct.packet], [32 x %struct.packet]* %30, i64 0, i64 1
  %32 = getelementptr inbounds %struct.packet, %struct.packet* %31, i32 0, i32 0
  store i64 3, i64* %32, align 8
  %33 = load %struct.pl_memory**, %struct.pl_memory*** %2, align 8
  %34 = getelementptr inbounds %struct.pl_memory*, %struct.pl_memory** %33, i64 1
  %35 = load %struct.pl_memory*, %struct.pl_memory** %34, align 8
  %36 = getelementptr inbounds %struct.pl_memory, %struct.pl_memory* %35, i32 0, i32 0
  %37 = getelementptr inbounds [32 x %struct.packet], [32 x %struct.packet]* %36, i64 0, i64 1
  %38 = getelementptr inbounds %struct.packet, %struct.packet* %37, i32 0, i32 1
  store i64 2, i64* %38, align 8
  %39 = load %struct.pl_memory**, %struct.pl_memory*** %2, align 8
  %40 = getelementptr inbounds %struct.pl_memory*, %struct.pl_memory** %39, i64 1
  %41 = load %struct.pl_memory*, %struct.pl_memory** %40, align 8
  %42 = getelementptr inbounds %struct.pl_memory, %struct.pl_memory* %41, i32 0, i32 0
  %43 = getelementptr inbounds [32 x %struct.packet], [32 x %struct.packet]* %42, i64 0, i64 1
  %44 = getelementptr inbounds %struct.packet, %struct.packet* %43, i32 0, i32 2
  store i64 4, i64* %44, align 8
  %45 = load %struct.pl_memory**, %struct.pl_memory*** %2, align 8
  %46 = getelementptr inbounds %struct.pl_memory*, %struct.pl_memory** %45, i64 1
  %47 = load %struct.pl_memory*, %struct.pl_memory** %46, align 8
  %48 = getelementptr inbounds %struct.pl_memory, %struct.pl_memory* %47, i32 0, i32 0
  %49 = getelementptr inbounds [32 x %struct.packet], [32 x %struct.packet]* %48, i64 0, i64 1
  %50 = getelementptr inbounds %struct.packet, %struct.packet* %49, i32 0, i32 3
  store i64 1, i64* %50, align 8
  %51 = load %struct.pl_memory**, %struct.pl_memory*** %2, align 8
  %52 = getelementptr inbounds %struct.pl_memory*, %struct.pl_memory** %51, i64 1
  %53 = load %struct.pl_memory*, %struct.pl_memory** %52, align 8
  %54 = getelementptr inbounds %struct.pl_memory, %struct.pl_memory* %53, i32 0, i32 0
  %55 = getelementptr inbounds [32 x %struct.packet], [32 x %struct.packet]* %54, i64 0, i64 30
  %56 = getelementptr inbounds %struct.packet, %struct.packet* %55, i32 0, i32 0
  store i64 3, i64* %56, align 8
  %57 = load %struct.pl_memory**, %struct.pl_memory*** %2, align 8
  %58 = getelementptr inbounds %struct.pl_memory*, %struct.pl_memory** %57, i64 0
  %59 = load %struct.pl_memory*, %struct.pl_memory** %58, align 8
  %60 = getelementptr inbounds %struct.pl_memory, %struct.pl_memory* %59, i32 0, i32 0
  %61 = getelementptr inbounds [32 x %struct.packet], [32 x %struct.packet]* %60, i64 0, i64 31
  %62 = getelementptr inbounds %struct.packet, %struct.packet* %61, i32 0, i32 3
  store i64 3, i64* %62, align 8
  ret void
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local i32 @caller_fn() #0 {
  %1 = alloca %struct.pl_memory*, align 8
  %2 = alloca %struct.pl_memory*, align 8
  %3 = alloca [2 x %struct.pl_memory*], align 16
  %4 = call noalias i8* @malloc(i64 1024) #2
  %5 = bitcast i8* %4 to %struct.pl_memory*
  store %struct.pl_memory* %5, %struct.pl_memory** %1, align 8
  %6 = call noalias i8* @malloc(i64 1024) #2
  %7 = bitcast i8* %6 to %struct.pl_memory*
  store %struct.pl_memory* %7, %struct.pl_memory** %2, align 8
  %8 = getelementptr inbounds [2 x %struct.pl_memory*], [2 x %struct.pl_memory*]* %3, i64 0, i64 0
  %9 = load %struct.pl_memory*, %struct.pl_memory** %1, align 8
  store %struct.pl_memory* %9, %struct.pl_memory** %8, align 8
  %10 = getelementptr inbounds %struct.pl_memory*, %struct.pl_memory** %8, i64 1
  %11 = load %struct.pl_memory*, %struct.pl_memory** %2, align 8
  store %struct.pl_memory* %11, %struct.pl_memory** %10, align 8
  %12 = getelementptr inbounds [2 x %struct.pl_memory*], [2 x %struct.pl_memory*]* %3, i32 0, i32 0
  call void @test_fn(%struct.pl_memory** %12)
  %13 = load %struct.pl_memory*, %struct.pl_memory** %1, align 8
  %14 = bitcast %struct.pl_memory* %13 to i8*
  call void @free(i8* %14) #2
  %15 = load %struct.pl_memory*, %struct.pl_memory** %2, align 8
  %16 = bitcast %struct.pl_memory* %15 to i8*
  call void @free(i8* %16) #2
  ret i32 0
}

; Function Attrs: nounwind
declare noalias i8* @malloc(i64) #1

; Function Attrs: nounwind
declare void @free(i8*) #1

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local i32 @main() #0 {
  %1 = call i32 @caller_fn()
  ret i32 0
}

attributes #0 = { noinline nounwind optnone sspstrong uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind }

!llvm.module.flags = !{!0, !1, !2}
!llvm.ident = !{!3}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{!"clang version 8.0.0 (tags/RELEASE_800/final)"}
