.. index::
   single: Forms; Fields; integer

.. note::

    * 対象バージョン：2.4 (2.2以降)
    * 翻訳更新日：2013/12/07

integer フィールドタイプ
========================

「数値」入力フィールドを表示します。基本的に、これは整数形式でのデータを扱うのが得意なテキストフィールドです。
``number`` 入力フィールドは、多くの特別なフロントエンド機能を有もつ HTML5 をサポートしているブラウザ場合以外はテキストボックスに見えます。

このフィールドは整数ではない入力値を処理する方法でさまざまなオプションを持っています。デフォルトでは、すべての非整数（例 6.78 )は( 6 のように)切り捨てします。

+-------------+-----------------------------------------------------------------------+
| 対応するタグ| ``input`` ``number`` field                                            |
+-------------+-----------------------------------------------------------------------+
| オプション  | - `rounding_mode`_                                                    |
|             | - `precision`_                                                        |
|             | - `grouping`_                                                         |
+-------------+-----------------------------------------------------------------------+
| 継承された  | - `required`_                                                         |
| オプション  | - `label`_                                                            |
|             | - `read_only`_                                                        |
|             | - `disabled`_                                                         |
|             | - `error_bubbling`_                                                   |
|             | - `error_mapping`_                                                    |
|             | - `invalid_message`_                                                  |
|             | - `invalid_message_parameters`_                                       |
|             | - `mapped`_                                                           |
+-------------+-----------------------------------------------------------------------+
| 親タイプ    | :doc:`form </reference/forms/types/form>`                             |
+-------------+-----------------------------------------------------------------------+
| クラス      | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\IntegerType` |
+-------------+-----------------------------------------------------------------------+

フィールドオプション
--------------------

.. include:: /reference/forms/types/options/precision.rst.inc

rounding_mode
~~~~~~~~~~~~~

**データ型**: ``integer`` **デフォルト**: ``IntegerToLocalizedStringTransformer::ROUND_DOWN``

デフォルトでは非整数が入力された場合は切り捨てられます。端数処理の方法は多数あり、それらは :class:`Symfony\\Component\\Form\\Extension\\Core\\DataTransformer\\IntegerToLocalizedStringTransformer` の定数です:

* ``IntegerToLocalizedStringTransformer::ROUND_DOWN`` 0 に向けて丸めます。

* ``IntegerToLocalizedStringTransformer::ROUND_FLOOR`` 負の無限大に向かって丸めます。

* ``IntegerToLocalizedStringTransformer::ROUND_UP`` 小数点以下切り上げます。

* ``IntegerToLocalizedStringTransformer::ROUND_CEILING`` 正の無限大方向に丸めます。

* ``IntegerToLocalizedStringTransformer::ROUND_HALF_DOWN`` 「もっとも近い数字」 に丸めます。両方から等距離にある場合、切捨てします。

* ``IntegerToLocalizedStringTransformer::ROUND_HALF_EVEN`` 「もっとも近い数字」 に丸めます。両方から等距離にある場合、末尾が偶数のほうに丸めます。

* ``IntegerToLocalizedStringTransformer::ROUND_HALF_UP`` 「もっとも近い数字」 に丸めます。両方から等距離にある場合、切り上げます。

.. include:: /reference/forms/types/options/grouping.rst.inc

継承されたオプション
--------------------

以下のオプションは :doc:`form </reference/forms/types/form>` タイプを継承しています:

.. include:: /reference/forms/types/options/required.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/disabled.rst.inc

.. include:: /reference/forms/types/options/error_bubbling.rst.inc

.. include:: /reference/forms/types/options/error_mapping.rst.inc

.. include:: /reference/forms/types/options/invalid_message.rst.inc

.. include:: /reference/forms/types/options/invalid_message_parameters.rst.inc

.. include:: /reference/forms/types/options/mapped.rst.inc

.. 2013/12/07 yositani2002 ba413f4f38b31ecc3db12ed9fcba8f62b3ae7f1f
