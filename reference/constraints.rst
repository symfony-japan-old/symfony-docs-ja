バリデータリファレンス
======================

.. toctree::
   :maxdepth: 1
   :hidden:

   constraints/NotBlank
   constraints/Blank
   constraints/NotNull
   constraints/Null
   constraints/True
   constraints/False
   constraints/Type

   constraints/Email
   constraints/MinLength
   constraints/MaxLength
   constraints/Url
   constraints/Regex
   constraints/Ip

   constraints/Max
   constraints/Min

   constraints/Date
   constraints/DateTime
   constraints/Time

   constraints/Choice
   constraints/Collection
   constraints/UniqueEntity
   constraints/Language
   constraints/Locale
   constraints/Country

   constraints/File
   constraints/Image

   constraints/Callback
   constraints/All
   constraints/UserPassword
   constraints/Valid

バリデータは、\ *制約* に対してオブジェクトを検証するように設計されています。
現実の世界では、制約はたとえば「ケーキは焼いてはいけない」となります。
Symfony2 の世界でも同様です。
制約とは、特定の状況を満たしていることを表明するものです。

サポートしている制約の一覧
--------------------------

Symfony2 では、次の制約が組み込みで利用できます。

.. include:: /reference/constraints/map.rst.inc

.. 2012/01/27 hidenorigoto f0b25f76d6e4637c1de5e5abdbab3d3bd0ee3c15

